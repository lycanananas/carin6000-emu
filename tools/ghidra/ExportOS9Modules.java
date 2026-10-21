// OS-9 Advanced Module Analysis Exporter for LLMs
// @category OS9
// @author CARiN_RE

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.CodeUnit;
import ghidra.program.model.listing.CodeUnitIterator;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSetView;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;

public class ExportOS9Modules extends GhidraScript {

    @Override
    public void run() throws Exception {
        // Output directory is the first script argument (see export_os9_modules.py).
        String[] args = getScriptArgs();
        File outputDir = new File(args.length > 0 ? args[0] : "os9_ghidra_exports");
        if (!outputDir.exists()) {
            outputDir.mkdirs();
        }

        String progName = currentProgram.getName();
        File outFile = new File(outputDir, progName + "_analysis.md");
        
        println("[*] Processing OS-9 Module: " + progName);

        // 1. Obsługa nagłówka OS-9 (0x4AFC)
        int syncBytes = getShort(toAddr(0x00)) & 0xFFFF;
        
        if (syncBytes == 0x4AFC) {
            // Offset 0x08 (4 bajty): Wskaźnik do punktu wejścia (Entry Point)
            int entryOffset = getInt(toAddr(0x08));
            // Offset 0x04 (2 bajty): Offset do nazwy modułu w ASCII
            int nameOffset = getShort(toAddr(0x04)) & 0xFFFF;

            Address entryAddr = toAddr(entryOffset);
            String modRealName = progName;

            try {
                // Bezpieczne odczytanie nazwy modułu z bajtów ASCII
                StringBuilder nameBuilder = new StringBuilder();
                Address currentNameAddr = toAddr(nameOffset);
                for (int i = 0; i < 32; i++) {
                    byte b = getByte(currentNameAddr.add(i));
                    if (b == 0 || b == 0x0D || (b & 0xFF) > 127) {
                        if ((b & 0x7F) >= 32) { // OS-9 czasem ustala ostatni bit nazwy na 1
                            nameBuilder.append((char)(b & 0x7F));
                        }
                        break;
                    }
                    if (b >= 32 && b <= 126) {
                        nameBuilder.append((char)b);
                    }
                }
                if (nameBuilder.length() > 0) {
                    modRealName = nameBuilder.toString();
                }
            } catch (Exception e) {
                modRealName = progName;
            }

            println("    [+] Found OS-9 Header! Real Name: " + modRealName + " | Entry Offset: " + entryAddr);

            // Wymuś utworzenie funkcji w prawdziwym punkcie wejścia, omijając nagłówek
            disassemble(entryAddr);
            createFunction(entryAddr, "os9_" + modRealName.replaceAll("[^a-zA-Z0-9_]", "_") + "_main");
        } else {
            println("    [!] Warning: 0x4AFC sync pattern not found at offset 0. Running standard analysis.");
        }

        // 2. Inicjalizacja dekompilatora
        DecompInterface decomp = new DecompInterface();
        decomp.toggleCCode(true);
        decomp.toggleSyntaxTree(true);
        decomp.openProgram(currentProgram);

        try (PrintWriter writer = new PrintWriter(new FileWriter(outFile))) {
            writer.println("# OS-9 Module Analysis: " + progName + "\n");
            writer.println("## Metadata");
            writer.println("- **Image Base:** " + currentProgram.getImageBase());
            writer.println("- **Min Address:** " + currentProgram.getMinAddress());
            writer.println("- **Max Address:** " + currentProgram.getMaxAddress());
            writer.println("\n---\n");

            FunctionIterator functions = currentProgram.getFunctionManager().getFunctions(true);
            int funcCount = 0;

            while (functions.hasNext()) {
                Function func = functions.next();
                funcCount++;
                
                writer.println("## Function: " + func.getName());
                writer.println("- **EntryPoint:** " + func.getEntryPoint() + "\n");

                // Decompiled C
                writer.println("### Decompiled C\n```c");
                DecompileResults result = decomp.decompileFunction(func, 60, monitor);
                if (result != null && result.decompileCompleted()) {
                    String cCode = result.getDecompiledFunction().getC();
                    if (cCode != null && !cCode.trim().isEmpty()) {
                        writer.println(cCode);
                    } else {
                        writer.println("// Decompilation output empty.");
                    }
                } else {
                    String errMsg = (result != null) ? result.getErrorMessage() : "Unknown error";
                    writer.println("// Decompilation incomplete/failed: " + errMsg);
                }
                writer.println("```\n");

                // Assembly Code
                writer.println("### Assembly\n```assembly");
                AddressSetView body = func.getBody();
                CodeUnitIterator codeUnits = currentProgram.getListing().getCodeUnits(body, true);
                while (codeUnits.hasNext()) {
                    CodeUnit cu = codeUnits.next();
                    writer.println(cu.getAddress() + ":  " + cu.toString());
                }
                writer.println("```\n");
                writer.println("---\n");
            }

            if (funcCount == 0) {
                writer.println("> **Note:** No executable functions identified in this module (likely raw data or table file).\n");
            }
        }
        println("[+] Export complete -> Saved to: " + outFile.getAbsolutePath());
    }
}