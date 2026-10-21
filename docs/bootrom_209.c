typedef unsigned char   undefined;

typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef unsigned short    ushort;


string s_loader.ini_000015f8;
undefined DAT_000018b7;
string s_/h0/loader.ini_00001959;
string s_/d0/loader.ini_00001968;
undefined1 *DAT_000004a4;
undefined4 DAT_000004a8;
int DAT_000004a4;
int DAT_000004a8;
byte DAT_00c0000f;
undefined DAT_00802400;
undefined DAT_00800000;
undefined1 DAT_00e0000a;
undefined1 DAT_00e00008;
undefined1 DAT_00e00009;
undefined DAT_0000637c;
undefined1 DAT_00e0000c;
undefined1 DAT_00c0001c;
undefined1 DAT_009f100d;
undefined DAT_009f1000;
undefined1 DAT_00c00001;
undefined DAT_00c00004;
undefined1 DAT_00c00028;
undefined DAT_009f100e;
undefined DAT_009f1012;
undefined DAT_009f1016;

undefined1 FUN_00000944(void)

{
  char *extraout_A0;
  char *in_A0;
  
  while (*in_A0 != '\0') {
    FUN_00000976();
    in_A0 = extraout_A0;
  }
  return 0;
}



void FUN_0000094a(void)

{
  return;
}



void FUN_00000960(void)

{
  return;
}



void FUN_00000976(void)

{
  return;
}



void FUN_0000098c(void)

{
  return;
}



void FUN_000009a2(void)

{
  return;
}



void FUN_000009b8(void)

{
  return;
}



void FUN_000009ce(void)

{
  return;
}



// WARNING: This function may have set the stack pointer
// WARNING: Removing unreachable block (ram,0x00005f10)
// WARNING: Removing unreachable block (ram,0x00005f22)
// WARNING: Removing unreachable block (ram,0x00005f32)
// WARNING: Removing unreachable block (ram,0x00005f38)
// WARNING: Removing unreachable block (ram,0x00005f48)
// WARNING: Removing unreachable block (ram,0x00005f4e)
// WARNING: Removing unreachable block (ram,0x00005f5a)
// WARNING: Removing unreachable block (ram,0x00005f6a)
// WARNING: Removing unreachable block (ram,0x00005f70)
// WARNING: Removing unreachable block (ram,0x00005f7c)
// WARNING: Removing unreachable block (ram,0x00005f8c)
// WARNING: Removing unreachable block (ram,0x00005fd0)
// WARNING: Removing unreachable block (ram,0x00005fee)
// WARNING: Removing unreachable block (ram,0x00006014)
// WARNING: Removing unreachable block (ram,0x00006022)
// WARNING: Removing unreachable block (ram,0x00006074)
// WARNING: Removing unreachable block (ram,0x0000608a)
// WARNING: Removing unreachable block (ram,0x000060a6)
// WARNING: Removing unreachable block (ram,0x000060d0)
// WARNING: Removing unreachable block (ram,0x000060d8)
// WARNING: Removing unreachable block (ram,0x000060de)
// WARNING: Removing unreachable block (ram,0x000060f8)
// WARNING: Removing unreachable block (ram,0x0000607e)
// WARNING: Removing unreachable block (ram,0x00005f8e)
// WARNING: Removing unreachable block (ram,0x00005eee)
// WARNING: Removing unreachable block (ram,0x00005ef8)
// WARNING: Removing unreachable block (ram,0x00006026)

void thunk_FUN_00005e68(void)

{
                    // WARNING: Do nothing block with infinite loop
                    // WARNING: Could not recover jumptable at 0x00006088. Too many branches
                    // WARNING: Treating indirect jump as call
  do {
  } while( true );
}



void FUN_00000a7a(void)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)FUN_000058fa();
                    // WARNING: Could not recover jumptable at 0x00000a82. Too many branches
                    // WARNING: Treating indirect jump as call
  (*(code *)*puVar1)();
  return;
}



undefined8 FUN_00000bfa(void)

{
  undefined4 in_D0;
  undefined4 in_D1;
  
  FUN_00000d38();
  return CONCAT44(in_D0,in_D1);
}



undefined4 FUN_00000d38(void)

{
  int *in_D0;
  char cVar2;
  undefined1 uVar3;
  int iVar1;
  undefined4 in_D1;
  int unaff_A6;
  undefined8 uVar4;
  int local_b8;
  undefined1 auStack_b0 [11];
  undefined1 auStack_a5 [5];
  undefined1 auStack_a0 [46];
  char *local_72;
  char local_6e;
  char local_6d;
  char local_6c;
  char local_6b;
  char local_6a;
  char local_69;
  char local_68;
  char local_67;
  int local_66;
  int local_62;
  int local_5e;
  int local_5a;
  int local_56;
  int aiStack_52 [10];
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int *local_18;
  undefined4 local_14;
  
  local_18 = in_D0;
  local_14 = in_D1;
  local_66 = FUN_00005a4a();
  local_68 = '\x01';
  local_69 = '\0';
  local_6a = '\0';
  local_6b = '\0';
  local_6c = '\0';
  local_6d = '\0';
  local_72 = s_loader_ini_000015f8;
  *(undefined4 *)(unaff_A6 + 0xa78) = local_14;
  FUN_00005838();
  cVar2 = FUN_00004422();
  if (cVar2 == '\x01') {
    thunk_FUN_00005e68();
  }
  uVar4 = FUN_000058fa();
  local_5e = *(int *)((int)((ulonglong)uVar4 >> 0x20) + -0x50 + (int)uVar4);
  if (local_5e == 0x4051ffff) {
    FUN_000058fa();
    uVar4 = FUN_000058fa();
    local_b8 = *(int *)((int)((ulonglong)uVar4 >> 0x20) + -0x3c + (int)uVar4);
  }
  else {
    FUN_00005922();
    uVar4 = FUN_000058fa();
    local_62 = *(int *)((int)((ulonglong)uVar4 >> 0x20) + -0x14 + (int)uVar4);
    if (local_62 == 0x4051ffff) {
      FUN_000058fa();
      FUN_00005922();
      uVar4 = FUN_000058fa();
      local_b8 = *(int *)((int)((ulonglong)uVar4 >> 0x20) + -0xc + (int)uVar4);
    }
    else if (local_5e != 0x4051ff00) {
      FUN_00005922();
      uVar4 = FUN_00005a7a();
      local_62 = *(int *)((int)((ulonglong)uVar4 >> 0x20) + -0x14 + (int)uVar4);
      if (local_62 == 0x4051ffff) {
        FUN_00005a7a();
      }
      else {
        FUN_00005922();
        uVar4 = FUN_00005a6a();
        local_62 = *(int *)((int)((ulonglong)uVar4 >> 0x20) + -0x14 + (int)uVar4);
        if (local_62 != 0x4051ffff) goto LAB_00000e54;
        FUN_00005a6a();
      }
      local_b8 = 0;
    }
  }
LAB_00000e54:
  FUN_00005992();
  uVar3 = FUN_0000455c();
  *(undefined1 *)(local_66 + 0x16) = uVar3;
  FUN_0000599a();
  uVar3 = FUN_0000455c();
  *(undefined1 *)(local_66 + 0x17) = uVar3;
  FUN_0000591a();
  uVar3 = FUN_0000455c();
  *(undefined1 *)(local_66 + 0x18) = uVar3;
  if (*(char *)(local_66 + 0x18) != '\0') {
    FUN_0000591a();
    FUN_00001dd6();
  }
  FUN_0000431a(10);
  FUN_00003ebe();
  FUN_00003ee6();
  FUN_00003e8c();
  iVar1 = FUN_00005a3a();
  if (*(char *)(iVar1 + 0xe) == '\x01') {
    local_6c = '\x01';
    FUN_00004098(1);
    local_68 = local_68 + '\x01';
  }
  FUN_00002904();
  iVar1 = FUN_00002038();
  if (iVar1 == 0) {
    local_6a = '\x01';
    FUN_00002122();
  }
  else {
    FUN_0000583e();
  }
  FUN_000034fe();
  FUN_00005748();
  FUN_00001500();
  FUN_0000583e();
  FUN_0000583e();
  *(undefined4 *)(local_66 + 0xe) = 0x5489a5;
  *(int *)(local_66 + 0x12) = local_18[1];
  if (local_5e == 0x4051ff00) {
    local_69 = '\x01';
    FUN_0000583e();
  }
  local_6e = FUN_0000143e();
  local_67 = FUN_000044a8();
  if ((((local_67 == '\0') && (local_6e != 'G')) &&
      ((local_62 == 0x4051ffff || (local_5e == 0x4051ffff)))) &&
     ((iVar1 = FUN_00001986(), -1 < iVar1 || (iVar1 = FUN_00001986(), -1 < iVar1)))) {
    FUN_000034fe();
    *local_18 = local_b8 + *local_18;
    local_18[1] = local_18[1] - local_b8;
    goto LAB_0000140e;
  }
  FUN_0000583e();
  FUN_000058ca();
  FUN_000058d2();
  iVar1 = FUN_00001986();
  if (iVar1 < 0) {
    FUN_0000583e();
    FUN_00001470();
  }
  FUN_000034fe();
  FUN_0000583e();
  if ((((local_69 == '\0') && (local_6a != '\0')) && (local_6e != 'G')) && (local_67 == '\0')) {
    FUN_0000583e();
    FUN_000034fe();
    iVar1 = FUN_0000220c(3);
    if (iVar1 == 0) {
      FUN_000034fe();
      FUN_0000583e();
      local_6b = '\x01';
      FUN_000014be();
    }
    else {
      FUN_00005a8a();
      FUN_000034fe();
      FUN_00005a8a();
    }
    FUN_0000583e();
  }
  FUN_00003406();
  FUN_000034fe();
  FUN_00005a8a();
  if ((local_69 != '\0') && (iVar1 = FUN_00003090(), iVar1 == 2)) {
    FUN_0000583e();
    FUN_00002ed2();
    FUN_00002f5e();
  }
  do {
    iVar1 = FUN_00003090();
  } while (iVar1 == 3);
  FUN_0000583e();
  local_20 = FUN_00003090();
  if (local_20 == 10) {
    FUN_00001562(auStack_a0,auStack_a5,(short)auStack_b0,(short)local_72,(char)&local_56);
    local_1c = FUN_000029c8();
    if (local_1c < 1) {
      FUN_00001562(auStack_a0,auStack_b0,(short)local_72,0,(char)&local_56);
      local_1c = FUN_000029c8();
      if (((local_1c < 1) && (local_1c = FUN_000029c8((char)&local_56), local_1c < 1)) &&
         (local_1c = FUN_000029c8((char)&local_56), local_1c < 1)) goto LAB_00001278;
    }
    FUN_000034fe();
    *local_18 = local_56;
    local_18[1] = local_1c;
    iVar1 = FUN_00005a5a();
    if (iVar1 == local_18[2]) {
      local_18[3] = 0x30;
      local_18[2] = local_56 + 2;
    }
    if ((((local_6a != '\0') && (local_6b != '\0')) && (local_6e != 'G')) &&
       ((local_67 == '\0' && (iVar1 = FUN_000014be(), iVar1 == -1)))) {
      FUN_0000583e();
    }
    FUN_0000342c();
    goto LAB_0000140e;
  }
LAB_00001278:
  FUN_0000583e((char)*(undefined4 *)(unaff_A6 + 0xa74));
  if (((local_20 == 10) && (local_69 == '\0')) &&
     ((local_24 = FUN_00002ed2(), local_24 == -1 || (local_24 != 0)))) {
    FUN_0000583e();
  }
  if (local_6b != '\0') {
    local_6b = '\0';
    FUN_00002470();
  }
  if ((local_6a == '\0') || (((local_67 != '\x01' && (local_6c == '\0')) && (local_6e != 'G')))) {
    if (local_69 == '\0') goto LAB_000013cc;
    FUN_00000a7a();
  }
  else {
    FUN_0000583e();
    local_28 = 0;
    local_5a = 0;
    FUN_00002122();
    FUN_00001562(&DAT_000018b7,auStack_a0,0x18b2,(short)(unaff_A6 + 0xa78),(char)&local_56);
    local_5a = FUN_00005518();
    if (local_5a < 1) {
      while ((aiStack_52[local_28 * 2] != 0 &&
             (local_5a = FUN_00005518((short)(unaff_A6 + 0xa78),(char)&local_56), local_5a < 1))) {
        local_28 = local_28 + 1;
      }
    }
    if (local_5a < 1) {
LAB_000013cc:
      local_6d = '\x01';
    }
    else {
      *local_18 = local_56;
      local_18[1] = local_5a;
      iVar1 = FUN_00005a5a();
      if (iVar1 == local_18[2]) {
        local_18[3] = 0x30;
        local_18[2] = local_56 + 2;
      }
    }
  }
  if (local_6d != '\0') {
    FUN_0000583e();
    FUN_0000583e();
    FUN_00004098(1);
    FUN_00001470();
  }
LAB_0000140e:
  if (local_6b != '\0') {
    FUN_00002470();
  }
  if (local_6a != '\0') {
    FUN_000020e6();
  }
  FUN_00004958();
  return *(undefined4 *)(unaff_A6 + 0xa78);
}



undefined4 FUN_0000143e(void)

{
  char cVar2;
  undefined4 uVar1;
  
  cVar2 = FUN_00003e06();
  if (cVar2 == 'G') {
    FUN_0000583e();
    FUN_00003e36();
    uVar1 = 0x47;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



void FUN_00001470(void)

{
  FUN_0000583e();
  FUN_0000583e();
  do {
    FUN_000034fe();
    FUN_00005a8a();
  } while( true );
}



undefined8 FUN_000014be(void)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 in_D1;
  
  iVar1 = FUN_00002342();
  if (iVar1 != -1) {
    iVar1 = FUN_00002342();
    if (iVar1 != -1) {
      uVar2 = 0;
      goto LAB_000014f4;
    }
  }
  uVar2 = 0xffffffff;
LAB_000014f4:
  return CONCAT44(uVar2,in_D1);
}



void FUN_00001500(void)

{
  undefined4 *in_D0;
  
  *in_D0 = s__h0_loader_ini_00001959;
  in_D0[1] = 0x45;
  in_D0[2] = s__d0_loader_ini_00001968;
  in_D0[3] = 0x7f;
  in_D0[4] = *in_D0;
  in_D0[5] = 0x1c;
  in_D0[6] = *in_D0;
  in_D0[7] = 0x7f;
  in_D0[8] = 0;
  return;
}



undefined1 *
FUN_00001562(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5)

{
  undefined1 *in_D0;
  int iVar1;
  int in_D1;
  int iVar2;
  
  *in_D0 = 0;
  for (iVar2 = 0; iVar2 < in_D1 + -1; iVar2 = iVar2 + 1) {
    iVar1 = FUN_00005c34(param_1,param_2,param_3,param_4,param_5);
    if (iVar1 != 0) {
      FUN_00005c6e();
      FUN_00005c6e();
    }
  }
  FUN_00005c6e();
  FUN_0000583e();
  return in_D0;
}



undefined8 FUN_00001986(void)

{
  bool bVar1;
  uint in_D0;
  char cVar3;
  char cVar4;
  int iVar2;
  undefined4 in_D1;
  undefined8 uVar5;
  undefined2 local_1e;
  undefined4 local_1c;
  
  bVar1 = false;
  local_1c = 0;
  FUN_0000583e();
  cVar3 = FUN_00003d4e();
  if ((((in_D0 & 0xffff) == 0) && (in_D0 < 0x1000000)) && (0x1fffff < in_D0)) {
    FUN_00003dba();
  }
  else {
    FUN_000059da();
    FUN_00003dba();
    FUN_000059da(0x10000);
    FUN_00005cfa();
    bVar1 = true;
  }
  FUN_00003da0();
  FUN_00004336();
  for (local_1e = 0; local_1e < 100; local_1e = local_1e + 1) {
    FUN_00005a8a();
    cVar4 = FUN_00003da0();
    if (cVar4 != '\0') {
      if (cVar4 == '\x02') {
        if (bVar1) {
          FUN_000059da(0x10000);
          FUN_00005cfa();
        }
        else {
          FUN_00003dba();
        }
        FUN_00003ddc();
      }
      else if (cVar4 == '\x03') break;
    }
  }
  FUN_00003da0();
  if (local_1e == 100) {
    local_1c = -1;
  }
  FUN_000059da();
  FUN_00003dba();
  FUN_000059da();
  uVar5 = FUN_000059e2();
  FUN_000059da((int)((ulonglong)uVar5 >> 0x20) - (int)uVar5);
  FUN_00005db2();
  FUN_000059ca();
  uVar5 = FUN_000059d2();
  FUN_00005a1a((int)((ulonglong)uVar5 >> 0x20) - (int)uVar5);
  FUN_000059ca();
  FUN_00005cfa();
  iVar2 = FUN_00003f68();
  if (iVar2 == 0) {
    iVar2 = FUN_00003f0e();
    if (iVar2 != -1) {
      FUN_0000583e();
      FUN_00003574();
      goto LAB_00001b0c;
    }
  }
  FUN_0000583e();
LAB_00001b0c:
  FUN_00003ddc();
  if (cVar3 != '\0') {
    FUN_00003d4e();
  }
  if (local_1c < 0) {
    FUN_0000583e();
  }
  return CONCAT44(local_1c,in_D1);
}



undefined8 FUN_00001bcc(void)

{
  undefined1 *puVar1;
  undefined4 in_D0;
  char cVar2;
  undefined4 in_D1;
  int unaff_A6;
  
  puVar1 = DAT_000004a4;
  cVar2 = FUN_0000455c();
  *(char *)(unaff_A6 + 0xa7c) = cVar2;
  if (cVar2 != '\0') {
    puVar1[5] = 0;
    puVar1[2] = 0x10;
    *puVar1 = 0x13;
    *puVar1 = 7;
    puVar1[1] = 0xbb;
    puVar1[2] = 5;
  }
  return CONCAT44(in_D0,in_D1);
}



undefined8 FUN_00001bf4(void)

{
  undefined1 *unaff_A3;
  undefined8 in_stack_00000000;
  
  unaff_A3[2] = 0x10;
  *unaff_A3 = 0x13;
  *unaff_A3 = 7;
  unaff_A3[1] = 0xbb;
  unaff_A3[2] = 5;
  return in_stack_00000000;
}



undefined4 FUN_00001c3a(void)

{
  undefined4 in_D0;
  char cVar2;
  undefined4 uVar1;
  int unaff_A6;
  
  cVar2 = FUN_0000455c();
  *(char *)(unaff_A6 + 0xa7c) = cVar2;
  if (cVar2 != '\0') {
    uVar1 = FUN_00001bf4();
    return uVar1;
  }
  return in_D0;
}



void FUN_00001c8e(void)

{
  return;
}



uint FUN_00001ca0(void)

{
  uint uVar1;
  int unaff_A6;
  
  if ((*(char *)(unaff_A6 + 0xa7c) == '\0') || ((*(byte *)(DAT_000004a4 + 1) & 1) == 0)) {
    uVar1 = 0xffffffff;
  }
  else {
    uVar1 = (uint)*(byte *)(DAT_000004a4 + 3);
  }
  return uVar1;
}



void FUN_00001cc2(void)

{
  int unaff_A6;
  
  do {
    if (*(char *)(unaff_A6 + 0xa7c) == '\0') break;
  } while ((*(byte *)(DAT_000004a8 + 1) & 1) == 0);
  FUN_00001c8e();
  return;
}



void FUN_00001d00(void)

{
  char cVar1;
  int iVar2;
  undefined1 in_D0b;
  short sVar3;
  int unaff_A6;
  
  iVar2 = DAT_000004a4;
  if (*(char *)(unaff_A6 + 0xa7c) != '\0') {
    if ((*(byte *)(DAT_000004a4 + 1) & 1) != 0) {
      cVar1 = *(char *)(DAT_000004a4 + 3);
      while (cVar1 == '\x13') {
        do {
        } while ((*(byte *)(DAT_000004a4 + 1) & 1) == 0);
        cVar1 = *(char *)(DAT_000004a4 + 3);
      }
    }
    *(undefined1 *)(DAT_000004a4 + 3) = in_D0b;
    do {
    } while ((*(byte *)(iVar2 + 1) & 4) == 0);
    sVar3 = 0x200;
    do {
      sVar3 = sVar3 + -1;
    } while (sVar3 != -1);
  }
  return;
}



void FUN_00001d6a(void)

{
  char cVar1;
  int iVar2;
  undefined1 in_D0b;
  short sVar3;
  int unaff_A6;
  
  iVar2 = DAT_000004a8;
  if (*(char *)(unaff_A6 + 0xa7c) != '\0') {
    if ((*(byte *)(DAT_000004a8 + 1) & 1) != 0) {
      cVar1 = *(char *)(DAT_000004a8 + 3);
      while (cVar1 == '\x13') {
        do {
        } while ((*(byte *)(DAT_000004a8 + 1) & 1) == 0);
        cVar1 = *(char *)(DAT_000004a8 + 3);
      }
    }
    *(undefined1 *)(DAT_000004a8 + 3) = in_D0b;
    do {
    } while ((*(byte *)(iVar2 + 1) & 4) == 0);
    sVar3 = 0x200;
    do {
      sVar3 = sVar3 + -1;
    } while (sVar3 != -1);
  }
  return;
}



void FUN_00001dd6(void)

{
  ushort uVar1;
  undefined4 in_D0;
  int *piVar2;
  undefined4 uVar3;
  int unaff_A6;
  
  piVar2 = (int *)FUN_00005a4a();
  *(undefined4 *)(unaff_A6 + 0xa7e) = in_D0;
  uVar3 = FUN_00001f68();
  *(undefined4 *)(unaff_A6 + 0xa82) = uVar3;
  if (*piVar2 != 0x5489a5) {
    **(undefined2 **)(unaff_A6 + 0xa7e) = 0xc169;
    *(undefined2 *)(piVar2 + 1) = 0xc169;
    *(undefined2 *)(*(int *)(unaff_A6 + 0xa7e) + 2) = 0x7400;
    *(undefined2 *)((int)piVar2 + 6) = 0x7400;
    *(undefined2 *)(*(int *)(unaff_A6 + 0xa7e) + 8) = 0x1400;
    *(undefined2 *)(piVar2 + 2) = 0x1400;
    *(undefined2 *)(*(int *)(unaff_A6 + 0xa7e) + 4) = 0;
    *(undefined4 *)((int)piVar2 + 10) = 0;
    *piVar2 = 0x5489a5;
  }
  FUN_00001f96();
  uVar1 = *(ushort *)((int)piVar2 + 6) | 0x30;
  *(ushort *)(*(int *)(unaff_A6 + 0xa7e) + 2) = uVar1;
  *(ushort *)((int)piVar2 + 6) = uVar1;
  FUN_00001f20();
  return;
}



void FUN_00001f20(void)

{
  FUN_00001f30();
  return;
}



void FUN_00001f30(void)

{
  short in_D0w;
  int iVar1;
  int unaff_A6;
  
  iVar1 = FUN_00005a4a();
  if (in_D0w == 1) {
    *(byte *)(iVar1 + 6) = *(byte *)(iVar1 + 6) | 0x80;
  }
  else {
    *(ushort *)(iVar1 + 6) = *(ushort *)(iVar1 + 6) & 0x7fff;
  }
  *(undefined2 *)(*(int *)(unaff_A6 + 0xa7e) + 2) = *(undefined2 *)(iVar1 + 6);
  return;
}



undefined8 FUN_00001f68(void)

{
  int iVar1;
  undefined4 in_D1;
  
  iVar1 = FUN_00001f80();
  return CONCAT44(iVar1 + -0x14,in_D1);
}



int FUN_00001f80(void)

{
  int iVar1;
  
  iVar1 = FUN_0000590a();
  return iVar1 + 0x400;
}



void FUN_00001f96(void)

{
  uint uVar1;
  int unaff_A6;
  
  **(undefined4 **)(unaff_A6 + 0xa82) = 0x70000000;
  *(undefined4 *)(*(int *)(unaff_A6 + 0xa82) + 4) = 0x78000400;
  *(uint *)(*(int *)(unaff_A6 + 0xa82) + 8) = *(int *)(unaff_A6 + 0xa82) + 0x10U | 0x20000000;
  uVar1 = FUN_0000590a();
  *(uint *)(*(int *)(unaff_A6 + 0xa82) + 0xc) = uVar1 | 0x50000000;
  *(uint *)(*(int *)(unaff_A6 + 0xa82) + 0x10) = *(int *)(unaff_A6 + 0xa82) + 0x10U | 0x30000000;
  FUN_00001ffc();
  return;
}



ulonglong FUN_00001ffc(void)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined4 in_D1;
  undefined8 uVar3;
  
  puVar2 = (undefined4 *)FUN_00001f80();
  uVar1 = *puVar2;
  uVar3 = FUN_00001f80();
  *(undefined4 *)((ulonglong)uVar3 >> 0x20) = (int)uVar3;
  return CONCAT44(uVar1,in_D1) & 0xffffffffffffff;
}



undefined8 FUN_00002038(void)

{
  uint *puVar1;
  int iVar2;
  undefined4 uVar3;
  undefined1 uVar4;
  undefined4 in_D1;
  int unaff_A6;
  undefined2 local_18;
  undefined2 local_16;
  undefined4 local_10;
  
  *(undefined4 *)(unaff_A6 + 0xb04) = 0;
  local_18 = 10;
  local_16 = 1;
  local_10 = in_D1;
  FUN_0000599a();
  iVar2 = FUN_0000455c();
  if (iVar2 == 0) {
    uVar3 = 0xffffffff;
  }
  else {
    FUN_0000599a(&local_18,unaff_A6 + 0xa86);
    iVar2 = FUN_00004f84();
    *(int *)(unaff_A6 + 0xa74) = iVar2;
    if (iVar2 == 0) {
      uVar4 = FUN_00005448();
      *(undefined1 *)(unaff_A6 + 0xb02) = uVar4;
      uVar3 = FUN_000053a4();
      *(undefined4 *)(unaff_A6 + 0xaca) = uVar3;
      puVar1 = (uint *)(unaff_A6 + 0xace);
      *puVar1 = *puVar1 >> 0x1e;
      *(byte *)puVar1 = *(byte *)puVar1 | 0x20;
      *(byte *)puVar1 = *(byte *)puVar1 & 0xef;
      *puVar1 = *puVar1 & 0xf000ffff | 0xb0000;
      *(undefined1 *)(unaff_A6 + 0xad0) = *(undefined1 *)(unaff_A6 + 0xb02);
      *(undefined4 *)(unaff_A6 + 0xad2) = 0;
      uVar3 = 0;
    }
    else {
      uVar3 = 0xffffffff;
    }
  }
  return CONCAT44(uVar3,local_10);
}



undefined4 FUN_000020e6(void)

{
  int iVar1;
  int unaff_A6;
  undefined4 local_10;
  
  local_10 = 0;
  **(undefined1 **)(unaff_A6 + 0xaa4) = 2;
  iVar1 = FUN_000050d8();
  if (iVar1 != 0) {
    local_10 = 0xffffffff;
  }
  return local_10;
}



void FUN_00002122(void)

{
  int in_D0;
  int unaff_A6;
  
  *(int *)(unaff_A6 + 0xb04) = in_D0 * 100;
  return;
}



undefined4 FUN_0000220c(undefined2 param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined1 in_D1b;
  int unaff_A6;
  
  *(undefined1 *)(unaff_A6 + 0xad1) = in_D1b;
  *(undefined1 *)(unaff_A6 + 0xad2) = 0;
  *(undefined4 *)(unaff_A6 + 0xad6) = 0x30000;
  *(undefined1 *)(unaff_A6 + 0xada) = 0;
  *(undefined1 *)(unaff_A6 + 0xadb) = *(undefined1 *)(unaff_A6 + 0xb02);
  *(undefined2 *)(unaff_A6 + 0xadc) = 10;
  *(undefined2 *)(unaff_A6 + 0xade) = param_1;
  iVar1 = FUN_00005c34();
  if (iVar1 < 0x12) {
    FUN_00005c52();
  }
  else {
    FUN_00005cfa(0x11);
    *(undefined1 *)(unaff_A6 + 0xaf1) = 0;
  }
  iVar1 = FUN_000024f2(8);
  if (iVar1 == 8) {
    if (*(short *)(unaff_A6 + 0xb00) < 0) {
      *(undefined4 *)(unaff_A6 + 0xa74) = *(undefined4 *)(unaff_A6 + 0xafa);
      uVar2 = 0xffffffff;
    }
    else {
      uVar2 = 0;
    }
  }
  else {
    uVar2 = 0xffffffff;
  }
  return uVar2;
}



undefined4 FUN_000022c8(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 in_D1;
  int unaff_A6;
  
  *(undefined1 *)(unaff_A6 + 0xad2) = *(undefined1 *)(unaff_A6 + 0xafe);
  *(undefined4 *)(unaff_A6 + 0xad6) = 0x40000;
  *(undefined4 *)(unaff_A6 + 0xada) = *(undefined4 *)(unaff_A6 + 0xafe);
  *(undefined4 *)(unaff_A6 + 0xade) = in_D1;
  uVar1 = FUN_000024f2();
  if ('\0' < *(char *)(unaff_A6 + 0xb0c)) {
    uVar2 = FUN_00005ad8(0);
    FUN_0000583e(uVar2);
  }
  return uVar1;
}



undefined4 FUN_00002342(void)

{
  int iVar1;
  undefined4 uVar2;
  uint in_D1;
  int unaff_A6;
  
  *(undefined1 *)(unaff_A6 + 0xad2) = *(undefined1 *)(unaff_A6 + 0xafe);
  *(undefined4 *)(unaff_A6 + 0xad6) = 0x50000;
  *(undefined4 *)(unaff_A6 + 0xada) = *(undefined4 *)(unaff_A6 + 0xafe);
  if (in_D1 < 0x11) {
    *(uint *)(unaff_A6 + 0xade) = in_D1;
  }
  else {
    *(undefined4 *)(unaff_A6 + 0xade) = 0x10;
  }
  FUN_00005cfa(*(undefined4 *)(unaff_A6 + 0xade));
  iVar1 = FUN_000024f2(8);
  if (iVar1 == 8) {
    if (*(int *)(unaff_A6 + 0xaf6) < 0) {
      *(undefined4 *)(unaff_A6 + 0xa74) = *(undefined4 *)(unaff_A6 + 0xaf2);
      uVar2 = 0xffffffff;
    }
    else {
      uVar2 = *(undefined4 *)(unaff_A6 + 0xaf6);
    }
  }
  else {
    uVar2 = 0xffffffff;
  }
  return uVar2;
}



undefined4 FUN_000023e8(void)

{
  ushort in_D0w;
  int iVar1;
  undefined4 uVar2;
  undefined4 in_D1;
  int unaff_A6;
  
  *(undefined1 *)(unaff_A6 + 0xad2) = *(undefined1 *)(unaff_A6 + 0xafe);
  *(undefined4 *)(unaff_A6 + 0xad6) = 0x70001;
  *(uint *)(unaff_A6 + 0xada) = (uint)in_D0w;
  *(undefined4 *)(unaff_A6 + 0xae2) = in_D1;
  *(undefined4 *)(unaff_A6 + 0xade) = *(undefined4 *)(unaff_A6 + 0xafe);
  iVar1 = FUN_000024f2(8);
  if (iVar1 == 8) {
    if (*(int *)(unaff_A6 + 0xaf6) < 0) {
      *(undefined4 *)(unaff_A6 + 0xa74) = *(undefined4 *)(unaff_A6 + 0xaf2);
      uVar2 = 0xffffffff;
    }
    else {
      uVar2 = *(undefined4 *)(unaff_A6 + 0xaf6);
    }
  }
  else {
    uVar2 = 0xffffffff;
  }
  return uVar2;
}



undefined8 FUN_00002470(void)

{
  int iVar1;
  undefined4 in_D1;
  int unaff_A6;
  undefined4 local_1c;
  int local_18;
  
  *(undefined1 *)(unaff_A6 + 0xad2) = *(undefined1 *)(unaff_A6 + 0xafe);
  *(undefined4 *)(unaff_A6 + 0xad6) = 0x60000;
  *(undefined4 *)(unaff_A6 + 0xada) = *(undefined4 *)(unaff_A6 + 0xafe);
  iVar1 = FUN_000024f2(8);
  if (iVar1 == 8) {
    if (local_18 < 0) {
      *(undefined4 *)(unaff_A6 + 0xa74) = local_1c;
      local_18 = -1;
    }
  }
  else {
    local_18 = -1;
  }
  return CONCAT44(local_18,in_D1);
}



int FUN_000024f2(int param_1)

{
  uint uVar1;
  bool bVar2;
  int iVar3;
  ushort uVar4;
  short sVar5;
  byte bVar6;
  int in_D1;
  int unaff_A6;
  int local_2c;
  undefined4 local_28;
  int local_24;
  int local_20;
  int local_18;
  
  local_2c = 0;
  bVar2 = false;
  *(undefined4 *)(unaff_A6 + 0xb08) = 0;
  local_18 = in_D1;
  local_20 = FUN_000053f6();
  **(byte **)(unaff_A6 + 0xaa4) = (byte)(local_20 << 3) | 4;
  iVar3 = FUN_0000282e(0x24);
  if (iVar3 == -1) {
    **(undefined1 **)(unaff_A6 + 0xaa4) = 2;
    *(undefined1 *)(unaff_A6 + 0xac0 + local_20) = 1;
    local_2c = -1;
  }
  else {
    do {
      while ((**(byte **)(unaff_A6 + 0xaa0) & 0x80) != 0x80) {
        if (*(int *)(unaff_A6 + 0xb04) != 0) {
          uVar1 = *(uint *)(unaff_A6 + 0xb08);
          *(int *)(unaff_A6 + 0xb08) = *(int *)(unaff_A6 + 0xb08) + 1;
          if (*(uint *)(unaff_A6 + 0xb04) < uVar1) {
            **(undefined1 **)(unaff_A6 + 0xaa4) = 2;
            *(undefined1 *)(unaff_A6 + 0xac0 + local_20) = 1;
            *(undefined4 *)(unaff_A6 + 0xa74) = 0x901;
            return -1;
          }
          FUN_00005a8a();
        }
      }
      iVar3 = -1;
      uVar4 = FUN_000052a8();
      if (uVar4 < 0xfe) {
        sVar5 = 0x100 - uVar4;
      }
      else {
        sVar5 = 0x200 - uVar4;
      }
      FUN_00005356(sVar5,&local_28,8);
      uVar4 = uVar4 - 8;
      if (local_28 >> 0x1e < 0x100) {
        bVar6 = (byte)(local_28 >> 0x1e);
        if (bVar6 == 0) {
          if ('\0' < *(char *)(unaff_A6 + 0xb0c)) {
            FUN_0000583e();
          }
          uVar1 = local_28 & 0x2fffffff;
          local_28._2_1_ = (undefined1)(uVar1 >> 8);
          local_28._2_2_ = CONCAT11(*(undefined1 *)(unaff_A6 + 0xb02),local_28._2_1_);
          local_28 = CONCAT22((short)(uVar1 >> 0x10),local_28._2_2_) | 0xa0000000;
          local_24 = 0x20000000;
          FUN_0000282e(8);
        }
        else if (bVar6 == 1) {
          if ((local_28 & 0xfffffff) >> 0x10 == 0xb) {
            if ((int)(local_28 << 2) < 0) {
              bVar2 = true;
            }
            else {
              iVar3 = FUN_000053f6();
              if (iVar3 != -1) {
                **(byte **)(unaff_A6 + 0xaa4) = (byte)(iVar3 << 3) | 4;
              }
            }
            if (param_1 < (int)(local_2c + (uint)uVar4)) {
              FUN_0000583e();
              FUN_0000583e(local_2c + (uint)uVar4);
              bVar2 = true;
              local_2c = -1;
            }
            else {
              FUN_00005356(sVar5 + 8,local_18,uVar4);
              local_18 = (uint)uVar4 + local_18;
              local_2c = (uint)uVar4 + local_2c;
            }
          }
        }
        else if (bVar6 == 2) {
          if ((local_28 & 0xfffffff) >> 0x10 == 0xb) {
            if (local_24 << 3 < 0) {
              bVar2 = true;
            }
            else if (local_24 << 2 < 0) {
              bVar2 = true;
              local_2c = -1;
            }
          }
          else if ('\0' < *(char *)(unaff_A6 + 0xb0c)) {
            FUN_0000583e(local_24);
          }
        }
      }
      if (iVar3 == -1) {
        if (bVar2) {
          *(undefined1 *)(unaff_A6 + 0xac0 + local_20) = 1;
        }
        else {
          **(byte **)(unaff_A6 + 0xaa4) = (byte)(local_20 << 3) | 4;
        }
      }
      else {
        *(undefined1 *)(unaff_A6 + 0xac0 + local_20) = 1;
        local_20 = iVar3;
      }
    } while (!bVar2);
  }
  return local_2c;
}



undefined4 FUN_0000282e(void)

{
  uint uVar1;
  int unaff_A6;
  
  FUN_00005100(*(undefined4 *)(unaff_A6 + 0xaca));
  **(byte **)(unaff_A6 + 0xaa4) = *(char *)(unaff_A6 + 0xacd) << 3 | 3;
  while( true ) {
    while ((**(byte **)(unaff_A6 + 0xaa0) & 1) != 1) {
      if (*(int *)(unaff_A6 + 0xb04) != 0) {
        uVar1 = *(uint *)(unaff_A6 + 0xb08);
        *(int *)(unaff_A6 + 0xb08) = *(int *)(unaff_A6 + 0xb08) + 1;
        if (*(uint *)(unaff_A6 + 0xb04) < uVar1) {
          **(undefined1 **)(unaff_A6 + 0xaa4) = 1;
          *(undefined4 *)(unaff_A6 + 0xa74) = 0x901;
          return 0xffffffff;
        }
        FUN_00005a8a();
      }
    }
    if ((**(byte **)(unaff_A6 + 0xaa0) & 2) == 2) break;
    **(byte **)(unaff_A6 + 0xaa4) = *(char *)(unaff_A6 + 0xacd) << 3 | 3;
  }
  return 0;
}



void FUN_00002904(void)

{
  undefined1 in_D0b;
  int unaff_A6;
  
  *(undefined1 *)(unaff_A6 + 0xb0c) = in_D0b;
  return;
}



int FUN_000029c8(uint *param_1)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  char cVar5;
  int iVar4;
  int unaff_A6;
  int local_30;
  int local_2c;
  int local_28;
  undefined4 local_24;
  int local_20;
  int local_1c;
  int local_18;
  uint local_14;
  
  if (*(int *)(unaff_A6 + 0xb0e) == 0) {
    iVar2 = FUN_000057ea(&local_1c);
    *(int *)(unaff_A6 + 0xb0e) = iVar2;
    if ((iVar2 == 0) || (local_1c < 0x804)) {
      return -0xcf;
    }
  }
  *(uint *)(unaff_A6 + 0xb0e) = *(int *)(unaff_A6 + 0xb0e) + 3U & 0xfffffffc;
  local_20 = *(int *)(unaff_A6 + 0xb0e);
  iVar2 = FUN_00003214(1);
  if (iVar2 == 1) {
    *(undefined1 *)(local_20 + 0x47) = 0;
    iVar2 = *(int *)(local_20 + 0x88);
    iVar4 = iVar2 + -1;
    if (iVar2 < 1) {
      iVar4 = iVar2 + 0x7fe;
    }
    FUN_0000583e();
    local_24 = *(undefined4 *)(unaff_A6 + 0xb0e);
    iVar3 = FUN_00003214(1);
    local_30 = (iVar4 >> 0xb) + 1;
    if (iVar3 == 1) {
      do {
        cVar5 = FUN_00002d38(iVar2);
        if (cVar5 != '\0') break;
        local_24 = *(undefined4 *)(unaff_A6 + 0xb0e);
        FUN_00003214(1);
        iVar4 = local_30 + -1;
        bVar1 = 0 < local_30;
        local_30 = iVar4;
      } while (iVar4 != 0 && bVar1);
      local_28 = *(int *)(unaff_A6 + 0xb0e);
      if ((cVar5 == '\0') || (iVar2 = FUN_00003214(1), iVar2 != 1)) {
        iVar2 = -0x832;
      }
      else {
        iVar2 = *(int *)(local_28 + 0xe);
        iVar4 = iVar2 + -1;
        if (iVar2 < 1) {
          iVar4 = iVar2 + 0x7fe;
        }
        local_30 = (iVar4 >> 0xb) + 1;
        if ((local_30 < 2) || (*(short *)(local_28 + 0x1a) == 0)) {
          do {
            iVar2 = FUN_00002cd4(&local_2c);
            if (iVar2 == 1) break;
            FUN_00003214(1);
            iVar2 = local_30 + -1;
            bVar1 = 0 < local_30;
            local_30 = iVar2;
          } while (bVar1);
          if (local_30 == 0) {
            iVar2 = -0xd7;
          }
          else {
            local_18 = *(int *)(local_2c + 0xe) + 4;
            local_14 = FUN_000057ea(&local_1c);
            if ((local_14 == 0) || (local_1c < local_18)) {
              iVar2 = -0xcf;
            }
            else {
              local_14 = local_14 + 4 & 0xfffffffc;
              iVar2 = *(int *)(local_2c + 0xe);
              iVar4 = iVar2 + -1;
              if (iVar2 < 1) {
                iVar4 = iVar2 + 0x7fe;
              }
              iVar2 = (iVar4 >> 0xb) + 1;
              iVar4 = FUN_00003214(iVar2);
              if (iVar4 == iVar2) {
                *param_1 = local_14;
                iVar2 = *(int *)(local_2c + 0xe);
                FUN_0000583e(local_14,iVar2);
              }
              else {
                iVar2 = -0xf4;
              }
            }
          }
        }
        else {
          iVar2 = -0x834;
        }
      }
    }
    else {
      iVar2 = -0x831;
    }
  }
  else {
    iVar2 = -0x830;
  }
  if (iVar2 < 0) {
    FUN_0000583e();
  }
  return iVar2;
}



undefined4 FUN_00002cd4(undefined4 *param_1)

{
  int iVar1;
  byte *in_D1;
  byte *local_14;
  
  local_14 = in_D1;
  do {
    local_14[local_14[0x20] + 0x21] = 0;
    iVar1 = FUN_00005bc0();
    if (iVar1 == 0) {
      *param_1 = local_14;
      return 1;
    }
    local_14 = local_14 + *local_14;
  } while (*local_14 != 0);
  return 0;
}



char FUN_00002d38(int param_1)

{
  bool bVar1;
  undefined4 *in_D0;
  byte bVar3;
  int iVar2;
  int *in_D1;
  byte *pbVar4;
  char *pcVar5;
  short local_26;
  short local_24;
  char local_21;
  
  pbVar4 = (byte *)*in_D1;
  local_21 = '\0';
  pcVar5 = (char *)*in_D0;
  local_26 = *(short *)(pbVar4 + 6);
  bVar1 = true;
  local_24 = local_26;
  do {
    if (bVar1) {
      *in_D0 = pcVar5;
      for (; (*pcVar5 != '\0' && (*pcVar5 != '/')); pcVar5 = pcVar5 + 1) {
      }
      if (*pcVar5 == '\0') {
        FUN_0000583e();
        local_21 = '\x01';
      }
      else {
        bVar1 = false;
        for (; *pcVar5 == '/'; pcVar5 = pcVar5 + 1) {
          *pcVar5 = '\0';
        }
      }
    }
    if (local_21 == '\0') {
      bVar3 = FUN_00005c34();
      if ((local_24 == *(short *)(pbVar4 + 6)) && (bVar3 == *pbVar4)) {
        iVar2 = FUN_00005ca2(bVar3);
        if (iVar2 == 0) {
          FUN_0000583e((int)local_26);
          local_24 = local_26;
          *in_D1 = (int)pbVar4;
          bVar1 = true;
        }
      }
      local_26 = local_26 + 1;
      pbVar4 = pbVar4 + (uint)(*pbVar4 & 1) + (uint)*pbVar4 + 8;
    }
  } while (((local_21 == '\0') && (pbVar4 < (byte *)(param_1 + *in_D1))) &&
          (*(short *)(pbVar4 + 6) != 0));
  return local_21;
}



void FUN_00002eba(void)

{
  FUN_00002f76();
  return;
}



undefined8 FUN_00002ed2(void)

{
  int iVar1;
  undefined4 in_D1;
  undefined4 local_12;
  undefined2 local_e;
  
  local_e = 0;
  local_12 = -2;
  while ((local_e < 0x1e && (local_12 == -2))) {
    iVar1 = FUN_00002eba();
    if (iVar1 == 0) {
      do {
        local_e = local_e + 1;
        FUN_00005a8a();
        iVar1 = FUN_00003090();
        if (iVar1 == 2) {
          local_12 = 0;
        }
      } while ((iVar1 != 10) && (local_e < 0x1e));
    }
    else {
      local_12 = -1;
    }
  }
  return CONCAT44(local_12,in_D1);
}



void FUN_00002f5e(void)

{
  FUN_00002f76();
  return;
}



undefined8 FUN_00002f76(void)

{
  short sVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 in_D1;
  int unaff_A6;
  char local_1b;
  undefined1 auStack_1a [2];
  short *local_18;
  undefined1 auStack_14 [8];
  undefined4 local_c;
  
  *(undefined4 *)(unaff_A6 + 0xa74) = 0;
  local_c = in_D1;
  iVar2 = FUN_00003bec(auStack_14,0xffffffff);
  *(int *)(unaff_A6 + 0xa74) = iVar2;
  if ((iVar2 == 0) && (iVar2 = FUN_000038c2(auStack_14), *(int *)(unaff_A6 + 0xa74) == iVar2)) {
    iVar2 = FUN_00003b4e();
    *(int *)(unaff_A6 + 0xa74) = iVar2;
    if (iVar2 == 0) {
      iVar2 = FUN_00003c80();
      *(int *)(unaff_A6 + 0xa74) = iVar2;
      if (iVar2 == 0) {
        iVar2 = FUN_00003a14();
        *(int *)(unaff_A6 + 0xa74) = iVar2;
        if (iVar2 == 0) {
          iVar2 = FUN_00003c46();
          *(uint *)(unaff_A6 + 0xa74) = (uint)(iVar2 != 0);
          if (((iVar2 != 0) == 0) && (local_1b == '\0')) {
            iVar2 = FUN_00003abc(auStack_1a);
            *(int *)(unaff_A6 + 0xa74) = iVar2;
            if (iVar2 == 0) goto LAB_0000304a;
          }
        }
      }
    }
  }
  if (*(int *)(unaff_A6 + 0xa74) == 0) {
    *(undefined4 *)(unaff_A6 + 0xa74) = 0x801;
  }
LAB_0000304a:
  FUN_00003b1a();
  if ((*(int *)(unaff_A6 + 0xa74) == 0) &&
     (sVar1 = *local_18, *(int *)(unaff_A6 + 0xa74) = (int)sVar1, sVar1 == 0)) {
    uVar3 = 0;
  }
  else {
    uVar3 = 0xffffffff;
  }
  return CONCAT44(uVar3,local_c);
}



undefined8 FUN_00003090(void)

{
  byte bVar1;
  int iVar2;
  undefined4 in_D1;
  int unaff_A6;
  undefined4 local_20;
  char local_1b;
  undefined1 auStack_1a [2];
  byte *local_18;
  undefined1 auStack_14 [8];
  undefined4 local_c;
  
  local_20 = 0xffffffff;
  *(undefined4 *)(unaff_A6 + 0xa74) = 0;
  local_c = in_D1;
  iVar2 = FUN_00003bec(auStack_14);
  *(int *)(unaff_A6 + 0xa74) = iVar2;
  if (iVar2 == 0) {
    iVar2 = FUN_00003b4e();
    *(int *)(unaff_A6 + 0xa74) = iVar2;
    if (iVar2 == 0) {
      iVar2 = FUN_00003c80();
      *(int *)(unaff_A6 + 0xa74) = iVar2;
      if (iVar2 == 0) {
        iVar2 = FUN_00003a14();
        *(int *)(unaff_A6 + 0xa74) = iVar2;
        if (iVar2 == 0) {
          iVar2 = FUN_00003c46();
          *(uint *)(unaff_A6 + 0xa74) = (uint)(iVar2 != 0);
          if (((iVar2 != 0) == 0) && (local_1b == '\x06')) {
            iVar2 = FUN_00003abc(auStack_1a);
            *(int *)(unaff_A6 + 0xa74) = iVar2;
            if (iVar2 == 0) {
              if ((*local_18 & 7) == 0) {
                bVar1 = local_18[2];
                if (bVar1 == 1) {
                  local_20 = 1;
                }
                else if ((bVar1 == 0) || (bVar1 == 2)) {
                  local_20 = 2;
                }
                else if (bVar1 == 3) {
                  local_20 = 3;
                }
                else if ((9 < bVar1) && (bVar1 < 0x12)) {
                  local_20 = 10;
                }
              }
              goto LAB_000031c4;
            }
          }
        }
      }
    }
  }
  if (*(int *)(unaff_A6 + 0xa74) == 0) {
    *(undefined4 *)(unaff_A6 + 0xa74) = 0x801;
  }
LAB_000031c4:
  FUN_00003b1a();
  return CONCAT44(local_20,local_c);
}



void FUN_000031f6(void)

{
  int unaff_A6;
  
  (**(code **)(unaff_A6 + 0xb12))();
  return;
}



int FUN_00003214(int param_1)

{
  short sVar1;
  bool bVar2;
  int in_D0;
  int iVar3;
  int iVar4;
  int in_D1;
  int unaff_A6;
  char local_27;
  int local_26;
  undefined1 auStack_1e [2];
  short *local_1c;
  undefined1 auStack_18 [4];
  int local_14;
  int local_10;
  int local_c;
  
  local_14 = 5;
  local_26 = param_1;
  bVar2 = false;
  local_10 = in_D0;
  local_c = in_D1;
  do {
    if (((local_26 == 0) || (local_14 < 1)) || (bVar2)) goto LAB_000033e0;
    local_14 = local_14 + -1;
    FUN_0000343e();
    FUN_000034a8();
    FUN_000034c4();
    *(undefined4 *)(unaff_A6 + 0xa74) = 0;
    iVar3 = FUN_00003bec(auStack_18);
    *(int *)(unaff_A6 + 0xa74) = iVar3;
    if (iVar3 == 0) {
      iVar3 = FUN_000038c2(auStack_18);
      *(int *)(unaff_A6 + 0xa74) = iVar3;
      if (iVar3 != 0) goto LAB_00003318;
      iVar3 = FUN_000038c2(auStack_18);
      *(int *)(unaff_A6 + 0xa74) = iVar3;
      if (iVar3 != 0) goto LAB_00003318;
      iVar3 = FUN_00003b4e();
      *(int *)(unaff_A6 + 0xa74) = iVar3;
      if (iVar3 != 0) goto LAB_00003318;
      iVar3 = FUN_00003c80();
      *(int *)(unaff_A6 + 0xa74) = iVar3;
      if (iVar3 != 0) goto LAB_00003318;
      iVar3 = FUN_00003a14();
      *(int *)(unaff_A6 + 0xa74) = iVar3;
      if (iVar3 != 0) goto LAB_00003318;
      iVar3 = FUN_00003c46();
      *(uint *)(unaff_A6 + 0xa74) = (uint)(iVar3 != 0);
      if (((iVar3 != 0) != 0) || (local_27 != '\0')) goto LAB_00003318;
      iVar3 = FUN_00003abc(auStack_1e);
      *(int *)(unaff_A6 + 0xa74) = iVar3;
      if (iVar3 != 0) goto LAB_00003318;
      sVar1 = *local_1c;
      *(int *)(unaff_A6 + 0xa74) = (int)sVar1;
      if ((sVar1 == 0) || (*(int *)(unaff_A6 + 0xa74) == 0x820)) {
        bVar2 = true;
      }
      else {
        if (*(int *)(unaff_A6 + 0xa74) != 0x828) {
          iVar4 = FUN_0000347c();
          iVar3 = iVar4 - local_c;
          if (iVar4 < local_c) {
            iVar3 = iVar3 + 0x7ff;
          }
          iVar3 = iVar3 >> 0xb;
          local_26 = local_26 - iVar3;
          local_10 = iVar3 + local_10;
          local_c = local_c + iVar3 * 0x800;
        }
        if ((local_26 != 0) && (*(int *)(unaff_A6 + 0xb12) != 0)) {
          FUN_000031f6();
        }
      }
    }
    else {
LAB_00003318:
      if (*(int *)(unaff_A6 + 0xa74) == 0) {
        *(undefined4 *)(unaff_A6 + 0xa74) = 0x801;
      }
      if (*(int *)(unaff_A6 + 0xa74) == 0xe4) {
        FUN_000034e2();
LAB_000033e0:
        if (*(int *)(unaff_A6 + 0xa74) != 0) {
          param_1 = 0;
        }
        return param_1;
      }
    }
    FUN_000034e2();
    FUN_00003b1a();
  } while( true );
}



void FUN_00003406(void)

{
  FUN_000035a2(1);
  return;
}



void FUN_0000342c(void)

{
  FUN_0000366c();
  return;
}



undefined4 FUN_0000343e(void)

{
  uint in_D0;
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_0000598a();
  if ((in_D0 & 1) == 0) {
    *(uint *)(iVar1 + 4) = in_D0;
    uVar2 = 0;
  }
  else {
    uVar2 = 0xffffffff;
  }
  return uVar2;
}



undefined4 FUN_0000347c(void)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = FUN_0000598a();
  iVar2 = FUN_0000598a(uVar1);
  return *(undefined4 *)(iVar2 + 4);
}



void FUN_000034a8(void)

{
  undefined4 in_D0;
  int iVar1;
  
  iVar1 = FUN_0000598a();
  *(undefined4 *)(iVar1 + 8) = in_D0;
  return;
}



void FUN_000034c4(void)

{
  int iVar1;
  
  iVar1 = FUN_0000598a();
  *(undefined1 *)(iVar1 + 0xc) = 0x80;
  return;
}



void FUN_000034e2(void)

{
  int iVar1;
  
  iVar1 = FUN_0000598a();
  *(undefined1 *)(iVar1 + 0xc) = 0;
  return;
}



undefined4 FUN_000034fe(void)

{
  int in_D0;
  int iVar1;
  undefined4 uVar2;
  undefined1 in_D1b;
  
  iVar1 = FUN_000059da();
  uVar2 = FUN_00003534();
  *(undefined1 *)(iVar1 + 0xa000 + in_D0) = in_D1b;
  return uVar2;
}



undefined8 FUN_00003534(void)

{
  int in_D0;
  int iVar1;
  undefined4 in_D1;
  undefined4 local_18;
  
  iVar1 = FUN_000059da();
  local_18 = (int)*(char *)(iVar1 + 0xa000 + in_D0);
  if (local_18 == -1) {
    local_18 = 3;
  }
  return CONCAT44(local_18,in_D1);
}



void FUN_00003574(void)

{
  int in_D0;
  int iVar1;
  undefined1 in_D1b;
  
  iVar1 = FUN_000059da();
  *(undefined1 *)(iVar1 + 0xa000 + in_D0) = in_D1b;
  return;
}



undefined4 FUN_000035a2(void)

{
  byte bVar1;
  byte *pbVar2;
  byte in_D0b;
  undefined1 *puVar3;
  
  puVar3 = (undefined1 *)FUN_000059da();
  pbVar2 = puVar3 + (short)(ushort)in_D0b * 6 + 2;
  *pbVar2 = in_D0b;
  bVar1 = in_D0b * '\x02' + 1;
  pbVar2[2] = bVar1;
  pbVar2[1] = bVar1;
  pbVar2[4] = pbVar2[2] + 1;
  pbVar2[3] = pbVar2[2] + 1;
  FUN_000036f6();
  if (puVar3[1] != 'i') {
    puVar3[1] = 0x69;
    FUN_0000378e(1000);
    FUN_000037aa();
  }
  *puVar3 = 0;
  return 0;
}



void FUN_0000366c(void)

{
  byte in_D0b;
  int iVar1;
  undefined2 local_12;
  
  iVar1 = FUN_000059da();
  if (*(char *)(iVar1 + 1) == 'i') {
    FUN_000036f6();
    *(undefined1 *)((short)(ushort)in_D0b * 6 + iVar1 + 2) = 0;
    for (local_12 = 0;
        ((short)local_12 < 10 && (*(byte *)(iVar1 + 2 + (short)local_12 * 6) != local_12));
        local_12 = local_12 + 1) {
    }
    if (local_12 == 10) {
      *(undefined1 *)(iVar1 + 1) = 0;
    }
  }
  return;
}



void FUN_000036f6(void)

{
  byte in_D0b;
  int iVar1;
  int in_D1;
  int local_18;
  undefined1 *local_14;
  
  iVar1 = FUN_000059da();
  local_14 = (undefined1 *)((uint)in_D0b * 0x100 + iVar1);
  for (local_18 = 0; local_18 < in_D1 * 0x100; local_18 = local_18 + 1) {
    *local_14 = 0;
    local_14 = local_14 + 1;
  }
  return;
}



void FUN_0000378e(undefined4 param_1)

{
  undefined4 in_D0;
  undefined4 in_D1;
  int unaff_A6;
  
  *(undefined4 *)(unaff_A6 + 0xb16) = in_D1;
  *(undefined4 *)(unaff_A6 + 0xb1a) = param_1;
  *(undefined4 *)(unaff_A6 + 0xb1e) = in_D0;
  return;
}



void FUN_000037aa(void)

{
  undefined4 in_D0;
  int unaff_A6;
  
  *(undefined4 *)(unaff_A6 + 0xb22) = in_D0;
  return;
}



undefined4 FUN_000037ba(byte *param_1)

{
  byte bVar1;
  byte *pbVar2;
  byte in_D0b;
  int iVar3;
  byte in_D1b;
  undefined4 local_26;
  
  local_26 = 0;
  iVar3 = FUN_000059da();
  pbVar2 = (byte *)((short)(ushort)in_D0b * 6 + iVar3 + 2);
  if ((*(char *)(iVar3 + 1) == 'i') && (in_D0b == *pbVar2)) {
    *param_1 = pbVar2[3];
    param_1[1] = pbVar2[1];
    *(ushort *)(param_1 + 2) = (((ushort)pbVar2[2] - (ushort)pbVar2[1]) + 1) * 0x100 + -8;
    bVar1 = param_1[1];
    iVar3 = FUN_000059da();
    pbVar2 = (byte *)((uint)bVar1 * 0x100 + iVar3);
    if (pbVar2[1] == 0) {
      pbVar2[1] = 1;
      *pbVar2 = in_D0b;
      pbVar2[2] = in_D1b;
      pbVar2[4] = 0;
      pbVar2[5] = 0;
      pbVar2[6] = 0;
      pbVar2[7] = 0;
    }
    else {
      local_26 = 0x301;
    }
  }
  else {
    local_26 = 0x302;
  }
  return local_26;
}



undefined4 FUN_000038c2(int param_1)

{
  short *psVar1;
  byte bVar2;
  char cVar6;
  int in_D0;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int local_1e;
  short sStack_12;
  
  bVar2 = *(byte *)(param_1 + 1);
  iVar3 = FUN_000059da();
  iVar3 = (uint)bVar2 * 0x100 + iVar3;
  if ((int)*(short *)(param_1 + 2) < in_D0 + *(short *)(iVar3 + 6)) {
    uVar4 = 0x306;
  }
  else {
    iVar5 = (int)*(short *)(iVar3 + 6);
    cVar6 = (char)((uint)(iVar5 + 8) >> 8);
    if (iVar5 < -8) {
      cVar6 = (char)((uint)(iVar5 + 0x107) >> 8);
    }
    local_1e = in_D0;
    if ((byte)(bVar2 + cVar6) == bVar2) {
      FUN_000059da();
      local_1e = 0xf8 - *(short *)(iVar3 + 6);
      FUN_00005cfa(local_1e);
      local_1e = in_D0 - local_1e;
    }
    for (; 0 < local_1e; local_1e = local_1e - iVar3) {
      FUN_000059da();
      iVar3 = local_1e;
      if (0x100 < local_1e) {
        iVar3 = 0x100;
      }
      FUN_00005cfa(iVar3);
    }
    iVar3 = FUN_000059da();
    sStack_12 = (short)in_D0;
    psVar1 = (short *)((uint)bVar2 * 0x100 + iVar3 + 6);
    *psVar1 = sStack_12 + *psVar1;
    uVar4 = 0;
  }
  return uVar4;
}



undefined4 FUN_00003a14(void)

{
  byte bVar1;
  byte in_D0b;
  int iVar2;
  undefined4 uVar3;
  byte *in_D1;
  
  iVar2 = FUN_000059da();
  iVar2 = (short)(ushort)in_D0b * 6 + iVar2 + 2;
  *in_D1 = *(byte *)(iVar2 + 3);
  in_D1[1] = *(byte *)(iVar2 + 1);
  *(ushort *)(in_D1 + 2) =
       (((ushort)*(byte *)(iVar2 + 4) - (ushort)*(byte *)(iVar2 + 3)) + 1) * 0x100;
  bVar1 = *in_D1;
  iVar2 = FUN_000059da();
  iVar2 = (uint)bVar1 * 0x100 + iVar2;
  if (*(char *)(iVar2 + 1) == '\x03') {
    *(undefined1 *)(iVar2 + 1) = 2;
    uVar3 = 0;
  }
  else {
    uVar3 = 0x307;
  }
  return uVar3;
}



undefined4 FUN_00003abc(undefined2 *param_1)

{
  byte bVar1;
  byte *in_D0;
  int iVar2;
  undefined4 uVar3;
  int *in_D1;
  
  bVar1 = *in_D0;
  iVar2 = FUN_000059da();
  iVar2 = (uint)bVar1 * 0x100 + iVar2;
  if (*(char *)(iVar2 + 1) == '\x02') {
    *in_D1 = iVar2 + 8;
    *param_1 = *(undefined2 *)(iVar2 + 6);
    uVar3 = 0;
  }
  else {
    uVar3 = 0x305;
  }
  return uVar3;
}



void FUN_00003b1a(void)

{
  byte bVar1;
  byte *in_D0;
  int iVar2;
  
  bVar1 = *in_D0;
  iVar2 = FUN_000059da();
  *(undefined1 *)((uint)bVar1 * 0x100 + iVar2 + 1) = 0;
  return;
}



undefined8 FUN_00003b4e(void)

{
  byte bVar1;
  bool bVar2;
  int in_D0;
  undefined4 uVar3;
  int iVar4;
  char cVar5;
  undefined4 in_D1;
  int unaff_A6;
  int iVar6;
  undefined4 local_1a;
  
  iVar6 = 0;
  if (*(int *)(unaff_A6 + 0xb16) == 0) {
    uVar3 = 0x309;
  }
  else {
    local_1a = *(int *)(unaff_A6 + 0xb1a);
    bVar1 = *(byte *)(in_D0 + 1);
    iVar4 = FUN_000059da();
    while( true ) {
      cVar5 = FUN_00003d84(iVar6,1);
      if (cVar5 == '\0') break;
      if ((*(int *)(unaff_A6 + 0xb1a) != 0) &&
         (bVar2 = local_1a < 1, local_1a = local_1a + -1, bVar2)) {
        uVar3 = 0x308;
        goto LAB_00003bde;
      }
      (**(code **)(unaff_A6 + 0xb1e))();
    }
    if (iVar6 == 0) {
      *(undefined1 *)((uint)bVar1 * 0x100 + iVar4 + 1) = 3;
      FUN_00003ddc();
    }
    uVar3 = 0;
  }
LAB_00003bde:
  return CONCAT44(uVar3,in_D1);
}



undefined4 FUN_00003bec(undefined4 param_1)

{
  bool bVar1;
  int iVar2;
  int unaff_A6;
  int local_14;
  
  local_14 = *(int *)(unaff_A6 + 0xb1a);
  do {
    iVar2 = FUN_000037ba(param_1);
    if (iVar2 == 0) {
      return 0;
    }
    (**(code **)(unaff_A6 + 0xb1e))();
  } while ((*(int *)(unaff_A6 + 0xb1a) == 0) ||
          (bVar1 = 0 < local_14, local_14 = local_14 + -1, bVar1));
  return 0x308;
}



undefined4 FUN_00003c46(void)

{
  byte bVar1;
  byte *in_D0;
  int iVar2;
  undefined1 *in_D1;
  
  bVar1 = *in_D0;
  iVar2 = FUN_000059da();
  *in_D1 = *(undefined1 *)((uint)bVar1 * 0x100 + iVar2 + 2);
  return 0;
}



void FUN_00003c80(void)

{
  int unaff_A6;
  
  (**(code **)(unaff_A6 + 0xb22))();
  return;
}



undefined1 FUN_00003d4e(void)

{
  undefined1 uVar1;
  undefined1 in_D0b;
  int iVar2;
  
  iVar2 = FUN_0000598a();
  uVar1 = *(undefined1 *)(iVar2 + 3);
  *(undefined1 *)(iVar2 + 3) = in_D0b;
  return uVar1;
}



undefined1 FUN_00003d84(void)

{
  int iVar1;
  
  iVar1 = FUN_0000598a();
  return *(undefined1 *)(iVar1 + 2);
}



undefined1 FUN_00003da0(void)

{
  undefined1 *puVar1;
  
  puVar1 = (undefined1 *)FUN_0000598a();
  return *puVar1;
}



void FUN_00003dba(void)

{
  int iVar1;
  undefined1 extraout_D1b;
  
  iVar1 = FUN_0000598a();
  *(undefined1 *)(iVar1 + 1) = extraout_D1b;
  return;
}



void FUN_00003ddc(void)

{
  undefined1 in_D0b;
  int iVar1;
  
  iVar1 = FUN_0000598a();
  *(undefined1 *)(iVar1 + 2) = in_D0b;
  return;
}



undefined8 FUN_00003e06(void)

{
  undefined4 uVar1;
  undefined4 in_D1;
  undefined1 local_11;
  
  uVar1 = FUN_0000402c(1);
  return CONCAT44(CONCAT31((int3)((uint)uVar1 >> 8),local_11),in_D1);
}



undefined8 FUN_00003e36(void)

{
  int iVar1;
  undefined4 in_D1;
  undefined4 local_14;
  
  local_14 = 0xffffffff;
  iVar1 = FUN_00004098(1);
  if (iVar1 != -1) {
    iVar1 = FUN_00004098(1);
    if (iVar1 != -1) {
      local_14 = 0;
    }
  }
  return CONCAT44(local_14,in_D1);
}



void FUN_00003e8c(void)

{
  undefined1 *in_D0;
  
  *in_D0 = 0;
  in_D0[9] = 0;
  FUN_0000402c(9);
  return;
}



void FUN_00003ebe(void)

{
  int in_D0;
  
  *(undefined1 *)(in_D0 + 5) = 0;
  FUN_0000402c(5);
  return;
}



void FUN_00003ee6(void)

{
  int in_D0;
  
  *(undefined1 *)(in_D0 + 4) = 0;
  FUN_0000402c(4);
  return;
}



void FUN_00003f0e(void)

{
  FUN_0000402c(1);
  return;
}



undefined8 FUN_00003f68(void)

{
  char cVar1;
  undefined4 in_D1;
  int local_34;
  char local_30;
  char local_26;
  
  local_34 = FUN_0000402c(0x1e);
  if (local_34 != -1) {
    if (local_30 != 'C') {
      cVar1 = FUN_00005ad8(0);
      if ((cVar1 != local_26) || (local_30 == '\0')) {
        local_34 = -1;
      }
    }
  }
  return CONCAT44(local_34,in_D1);
}



undefined4 FUN_0000402c(int param_1)

{
  int in_D0;
  int iVar1;
  int iVar2;
  undefined1 *in_D1;
  undefined1 *puVar3;
  int unaff_A6;
  undefined1 *local_14;
  
  iVar1 = FUN_0000596a();
  iVar2 = FUN_00005a4a();
  while (*(char *)(iVar2 + 0x19) != '\0') {
    (**(code **)(unaff_A6 + 0xb2e))();
  }
  *(undefined1 *)(iVar2 + 0x19) = 1;
  puVar3 = (undefined1 *)(in_D0 + iVar1);
  local_14 = in_D1;
  while (param_1 != 0) {
    *local_14 = *puVar3;
    puVar3 = puVar3 + 1;
    param_1 = param_1 + -1;
    local_14 = local_14 + 1;
  }
  *(undefined1 *)(iVar2 + 0x19) = 0;
  return 0;
}



int FUN_00004098(uint param_1)

{
  int in_D0;
  int iVar1;
  int iVar2;
  char cVar3;
  undefined1 *in_D1;
  uint uVar4;
  int unaff_A6;
  undefined1 auStack_e6 [198];
  int local_20;
  int local_1c;
  undefined1 *local_18;
  
  local_1c = in_D0;
  local_18 = in_D1;
  iVar1 = FUN_0000596a();
  local_20 = 0;
  iVar2 = FUN_00005a4a();
  while (*(char *)(iVar2 + 0x19) != '\0') {
    (**(code **)(unaff_A6 + 0xb2e))();
  }
  *(undefined1 *)(iVar2 + 0x19) = 1;
  if (param_1 == 1) {
    *(undefined1 *)(local_1c + iVar1) = *local_18;
    local_20 = FUN_000042ce();
    if (local_20 == -1) {
      *(undefined1 *)(local_1c + iVar1) = *local_18;
      local_20 = FUN_000042ce();
    }
  }
  else {
    for (; (local_20 == 0 && (0 < (int)param_1)); param_1 = param_1 - uVar4) {
      uVar4 = 0x20 - local_1c % 0x20;
      if (param_1 < uVar4) {
        uVar4 = param_1;
      }
      cVar3 = FUN_000041ac(uVar4,auStack_e6);
      if (cVar3 != '\0') {
        iVar1 = FUN_00004238();
        if (iVar1 != 0) {
          iVar1 = FUN_00004238();
          if (iVar1 != 0) {
            local_20 = -1;
          }
        }
      }
      local_1c = uVar4 + local_1c;
      local_18 = local_18 + uVar4;
    }
  }
  *(undefined1 *)(iVar2 + 0x19) = 0;
  return local_20;
}



bool FUN_000041ac(int param_1,int param_2)

{
  char cVar1;
  int in_D0;
  int iVar2;
  char *in_D1;
  int iVar3;
  char *pcVar4;
  int local_20;
  char *local_18;
  
  iVar3 = 0;
  iVar2 = FUN_0000596a();
  pcVar4 = (char *)(in_D0 + iVar2);
  local_18 = in_D1;
  for (local_20 = 0; local_20 < param_1; local_20 = local_20 + 1) {
    cVar1 = *local_18;
    if (cVar1 != *pcVar4) {
      *(char **)(param_2 + iVar3 * 6) = pcVar4;
      iVar2 = iVar3 * 6;
      iVar3 = iVar3 + 1;
      *(char *)(param_2 + 4 + iVar2) = cVar1;
    }
    pcVar4 = pcVar4 + 1;
    local_18 = local_18 + 1;
  }
  *(undefined4 *)(param_2 + iVar3 * 6) = 0;
  return iVar3 != 0;
}



void FUN_00004238(void)

{
  int *in_D0;
  undefined4 uVar1;
  
  uVar1 = FUN_00005b2c();
  for (; (undefined1 *)*in_D0 != (undefined1 *)0x0; in_D0 = (int *)((int)in_D0 + 6)) {
    *(undefined1 *)*in_D0 = *(undefined1 *)(in_D0 + 1);
  }
  FUN_00005bb4(uVar1);
  FUN_000042ce();
  return;
}



undefined4 FUN_000042ce(void)

{
  char *in_D0;
  char in_D1b;
  int iVar1;
  int unaff_A6;
  undefined4 local_18;
  
  local_18 = 0;
  iVar1 = 0;
  while ((iVar1 < *(int *)(unaff_A6 + 0xb26) && (in_D1b != *in_D0))) {
    (**(code **)(unaff_A6 + 0xb2e))();
    iVar1 = iVar1 + -1;
  }
  if (in_D1b != *in_D0) {
    local_18 = 0xffffffff;
  }
  return local_18;
}



void FUN_0000431a(undefined4 param_1)

{
  undefined4 in_D0;
  undefined4 in_D1;
  int unaff_A6;
  
  *(undefined4 *)(unaff_A6 + 0xb2a) = in_D1;
  *(undefined4 *)(unaff_A6 + 0xb26) = param_1;
  *(undefined4 *)(unaff_A6 + 0xb2e) = in_D0;
  return;
}



undefined8 FUN_00004336(void)

{
  byte bVar1;
  undefined4 in_D0;
  char cVar3;
  short sVar2;
  undefined4 in_D1;
  short sVar4;
  
  cVar3 = FUN_000043b2();
  sVar4 = 1;
  bVar1 = DAT_00c0000f | 0x40;
  do {
    DAT_00c0000f = bVar1;
    sVar2 = 0xff;
    do {
      sVar2 = sVar2 + -1;
    } while (sVar2 != -1);
    sVar4 = sVar4 + -1;
    bVar1 = DAT_00c0000f & 0xbf;
  } while (sVar4 != -1);
  if (cVar3 != '\x01') {
    FUN_00004396();
  }
  return CONCAT44(in_D0,in_D1);
}



undefined4 FUN_00004396(void)

{
  uint in_CACR;
  
  if ((in_CACR & 1) != 0) {
    return 1;
  }
  return 0;
}



undefined4 FUN_000043b2(void)

{
  uint in_CACR;
  
  if ((in_CACR & 1) == 0) {
    return 0;
  }
  return 1;
}



undefined4 FUN_0000440c(void)

{
  undefined4 in_D0;
  
  return in_D0;
}



undefined8 FUN_00004422(void)

{
  short sVar2;
  undefined4 uVar1;
  undefined4 in_D1;
  short local_10;
  undefined1 uVar3;
  
  sVar2 = FUN_0000477e(0);
  FUN_00004b20();
  uVar1 = FUN_00004b4e();
  if (sVar2 == 0) {
    uVar3 = 1;
  }
  else {
    local_10 = FUN_00004696();
    if (-1 < local_10) {
      local_10 = FUN_0000459a();
    }
    if (local_10 < 0) {
      uVar1 = FUN_00004510();
      uVar3 = (undefined1)uVar1;
    }
    else {
      uVar3 = local_10 == 0;
      uVar1 = 0;
    }
  }
  return CONCAT44(CONCAT31((int3)((uint)uVar1 >> 8),uVar3),in_D1);
}



undefined8 FUN_000044a8(void)

{
  undefined4 uVar1;
  undefined4 in_D1;
  undefined1 uVar2;
  short local_10;
  
  local_10 = FUN_00004696(0);
  if (-1 < local_10) {
    local_10 = FUN_0000459a();
  }
  if (local_10 < 0) {
    uVar1 = FUN_00004510();
    uVar2 = (undefined1)uVar1;
  }
  else {
    uVar2 = local_10 == 0;
    uVar1 = 0;
  }
  return CONCAT44(CONCAT31((int3)((uint)uVar1 >> 8),uVar2),in_D1);
}



undefined8 FUN_00004510(void)

{
  byte in_D0b;
  int iVar1;
  uint uVar2;
  uint uVar3;
  undefined4 in_D1;
  undefined1 local_11;
  
  iVar1 = FUN_00005992();
  local_11 = 0;
  uVar2 = FUN_0000455c();
  uVar3 = 0;
  if ((uVar2 != 0) && (uVar3 = uVar2 & 0xffffff00, (in_D0b & *(byte *)(iVar1 + 0xd)) == 0)) {
    local_11 = 1;
  }
  return CONCAT44(CONCAT31((int3)(uVar3 >> 8),local_11),in_D1);
}



undefined4 FUN_0000455c(void)

{
  undefined1 *in_D0;
  char extraout_D1b;
  
  FUN_00004a2e();
  if ((extraout_D1b != '\x01') && (extraout_D1b == '\x02')) {
    *in_D0 = *in_D0;
  }
  FUN_00004a2e();
  return 1;
}



uint FUN_0000459a(void)

{
  undefined4 uVar1;
  uint uVar2;
  ushort uVar3;
  byte in_D1b;
  int unaff_A6;
  
  if (*(char *)(unaff_A6 + 0xb32) == '\0') {
    uVar1 = FUN_0000582a();
    *(undefined4 *)(unaff_A6 + 0xb34) = uVar1;
    if (*(int *)(unaff_A6 + 0xb34) != 0) {
      *(undefined1 *)(unaff_A6 + 0xb32) = 1;
    }
  }
  if (*(char *)(unaff_A6 + 0xb32) == '\0') {
    uVar2 = 0xfffffffe;
  }
  else {
    FUN_00004e16();
    FUN_00004a88();
    FUN_00004ad4();
    FUN_00004d40();
    FUN_00005a8a();
    uVar3 = FUN_00004bd8();
    FUN_00004b7c();
    if ((uVar3 == 0x283) || (uVar3 == 0x2a3)) {
      uVar2 = (uint)(in_D1b & **(byte **)(unaff_A6 + 0xb34));
    }
    else if ((uVar3 == 0x300) || (uVar3 == 0x310)) {
      uVar2 = 0xf4ff;
    }
    else if ((uVar3 & 8) == 0) {
      if ((uVar3 & 0x10) == 0) {
        uVar2 = 0xffffffff;
      }
      else {
        FUN_00004b20();
        FUN_00004b4e();
        uVar2 = 0xf4fd;
      }
    }
    else {
      FUN_00004b20();
      FUN_00004b4e();
      uVar2 = 0xf4fe;
    }
  }
  return uVar2;
}



undefined4 FUN_00004696(void)

{
  ushort uVar2;
  undefined4 uVar1;
  
  FUN_00004e16();
  FUN_00004a88();
  FUN_00004ad4();
  FUN_00004d40();
  FUN_00005a8a();
  uVar2 = FUN_00004bd8();
  FUN_00004b7c();
  if ((uVar2 == 899) || (uVar2 == 0x3a3)) {
    uVar1 = 0;
  }
  else if ((uVar2 == 0x300) || (uVar2 == 0x310)) {
    uVar1 = 0xf4ff;
  }
  else if ((uVar2 == 0x381) || (uVar2 == 0x3a1)) {
    uVar1 = 0xf4fc;
  }
  else if ((uVar2 & 8) == 0) {
    if ((uVar2 & 0x10) == 0) {
      uVar1 = 0xffffffff;
    }
    else {
      FUN_00004b20();
      FUN_00004b4e();
      uVar1 = 0xf4fd;
    }
  }
  else {
    FUN_00004b20();
    FUN_00004b4e();
    uVar1 = 0xf4fe;
  }
  return uVar1;
}



undefined8 FUN_0000477e(void)

{
  bool bVar1;
  undefined4 uVar2;
  short sVar3;
  char cVar4;
  undefined4 in_D1;
  int unaff_A6;
  int local_1e;
  char *local_1a;
  
  bVar1 = false;
  local_1e = 0;
  while ((local_1e < 100 && (!bVar1))) {
    FUN_00005a8a();
    sVar3 = FUN_00004bd8();
    if ((sVar3 == 0x40) || (((sVar3 == 0x42 || (sVar3 == 0x60)) || (sVar3 == 0x62)))) {
      bVar1 = true;
    }
    local_1e = local_1e + 1;
  }
  if (bVar1) {
    FUN_00004b7c();
    if (*(char *)(unaff_A6 + 0xb32) == '\0') {
      uVar2 = FUN_0000582a();
      *(undefined4 *)(unaff_A6 + 0xb34) = uVar2;
      if (*(int *)(unaff_A6 + 0xb34) != 0) {
        *(undefined1 *)(unaff_A6 + 0xb32) = 1;
        for (local_1a = *(char **)(unaff_A6 + 0xb34);
            local_1a < (undefined1 *)(*(int *)(unaff_A6 + 0xb34) + 5); local_1a = local_1a + 1) {
          *local_1a = 0xff;
        }
      }
    }
    if (*(char *)(unaff_A6 + 0xb32) == '\0') {
      uVar2 = 0xfffffffe;
    }
    else {
      FUN_00004a88();
      FUN_00004ad4();
      FUN_00004ebe();
      FUN_00004e62();
      FUN_00005a8a();
      sVar3 = FUN_00004bd8();
      FUN_00004b7c();
      if ((sVar3 == 0x43) || (sVar3 == 99)) {
        cVar4 = FUN_00004af8();
        if (cVar4 == '\0') {
          bVar1 = true;
          for (local_1a = *(char **)(unaff_A6 + 0xb34);
              local_1a < (char *)(*(int *)(unaff_A6 + 0xb34) + 5); local_1a = local_1a + 1) {
            if (*local_1a != '\0') {
              bVar1 = false;
              break;
            }
          }
          if (bVar1) {
            uVar2 = 0;
          }
          else {
            uVar2 = 0xffffffff;
          }
        }
        else {
          uVar2 = 0xffffffff;
        }
      }
      else {
        uVar2 = 0xffffffff;
      }
    }
  }
  else {
    uVar2 = 0xffffffff;
  }
  return CONCAT44(uVar2,in_D1);
}



void FUN_00004958(void)

{
  undefined1 in_D0b;
  int iVar1;
  
  iVar1 = FUN_0000598a();
  *(undefined1 *)(iVar1 + 0x1c) = in_D0b;
  return;
}



undefined4 FUN_00004a2e(void)

{
  undefined4 uVar1;
  undefined4 in_D0;
  int iVar2;
  
  iVar2 = FUN_000059b2();
  uVar1 = *(undefined4 *)(iVar2 + 8);
  *(undefined4 *)(iVar2 + 8) = in_D0;
  return uVar1;
}



void FUN_00004a66(void)

{
  FUN_00005a4a(0x1a);
  FUN_00005db2();
  return;
}



void FUN_00004a88(void)

{
  undefined4 in_D0;
  int iVar1;
  
  iVar1 = FUN_0000598a();
  *(undefined4 *)(iVar1 + 0x20) = in_D0;
  return;
}



void FUN_00004ad4(void)

{
  undefined1 in_D0b;
  int iVar1;
  
  iVar1 = FUN_0000598a();
  *(undefined1 *)(iVar1 + 0x28) = in_D0b;
  return;
}



undefined1 FUN_00004af8(void)

{
  int iVar1;
  
  iVar1 = FUN_0000598a();
  return *(undefined1 *)(iVar1 + 0x28);
}



void FUN_00004b20(void)

{
  int iVar1;
  
  iVar1 = FUN_0000598a();
  *(ushort *)(iVar1 + 0x24) = *(ushort *)(iVar1 + 0x24) | 0x200;
  return;
}



void FUN_00004b4e(void)

{
  int iVar1;
  
  iVar1 = FUN_0000598a();
  *(ushort *)(iVar1 + 0x24) = *(ushort *)(iVar1 + 0x24) & 0xfdff;
  return;
}



void FUN_00004b7c(void)

{
  int iVar1;
  
  iVar1 = FUN_0000598a();
  *(ushort *)(iVar1 + 0x24) = *(ushort *)(iVar1 + 0x24) | 4;
  return;
}



undefined2 FUN_00004bd8(void)

{
  int iVar1;
  
  iVar1 = FUN_0000598a();
  return *(undefined2 *)(iVar1 + 0x26);
}



void FUN_00004d40(void)

{
  int iVar1;
  
  iVar1 = FUN_0000598a();
  *(ushort *)(iVar1 + 0x24) = *(ushort *)(iVar1 + 0x24) | 0x80;
  return;
}



void FUN_00004e16(void)

{
  undefined1 in_D0b;
  int iVar1;
  
  iVar1 = FUN_0000598a();
  *(undefined1 *)(iVar1 + 0x2a) = in_D0b;
  return;
}



void FUN_00004e62(void)

{
  int iVar1;
  
  iVar1 = FUN_0000598a();
  *(ushort *)(iVar1 + 0x24) = *(ushort *)(iVar1 + 0x24) | 0x20;
  return;
}



undefined4 FUN_00004ebe(void)

{
  char in_D0b;
  int iVar1;
  undefined4 local_14;
  
  local_14 = 0;
  iVar1 = FUN_0000598a();
  if (in_D0b == '\0') {
    *(ushort *)(iVar1 + 0x24) = *(ushort *)(iVar1 + 0x24) & 0xfff7;
  }
  else if (in_D0b == '\x01') {
    *(ushort *)(iVar1 + 0x24) = *(ushort *)(iVar1 + 0x24) | 8;
  }
  else {
    local_14 = 0xffffffff;
  }
  return local_14;
}



undefined4 FUN_00004f84(undefined2 *param_1,int param_2)

{
  int in_D0;
  undefined4 uVar1;
  char in_D1b;
  int local_20;
  
  *(undefined2 *)(param_2 + 0x12) = *param_1;
  *(int *)(param_2 + 0x14) = in_D0;
  *(int *)(param_2 + 0x1a) = in_D0;
  *(int *)(param_2 + 0x1e) = in_D0 + 1;
  *(int *)(param_2 + 0x22) = in_D0 + 1;
  *(int *)(param_2 + 0x26) = in_D0 + 2;
  *(int *)(param_2 + 0x2a) = in_D0 + 3;
  *(int *)(param_2 + 0x2e) = in_D0 + 4;
  *(int *)(param_2 + 0x32) = in_D0 + 6;
  *(int *)(param_2 + 0x36) = in_D0 + 7;
  for (local_20 = 0; local_20 < 4; local_20 = local_20 + 1) {
    *(undefined1 *)(local_20 + param_2 + 0x3a) = 1;
  }
  if (in_D1b == '\0') {
    uVar1 = 0xffffffff;
  }
  else {
    FUN_0000547c();
    FUN_000054e0();
    **(byte **)(param_2 + 0x32) = **(byte **)(param_2 + 0x32) | 0x20;
    *(undefined1 *)(param_2 + 0x18) = 0;
    **(undefined1 **)(param_2 + 0x14) = 0;
    **(undefined1 **)(param_2 + 0x1e) = 1;
    **(undefined1 **)(param_2 + 0x1e) = 2;
    **(undefined1 **)(param_2 + 0x1e) = 0x1e;
    if (param_1[1] == 1) {
      **(undefined1 **)(param_2 + 0x1e) = 0xd;
    }
    else {
      **(undefined1 **)(param_2 + 0x1e) = 5;
    }
    uVar1 = 0;
  }
  return uVar1;
}



undefined4 FUN_000050d8(void)

{
  int in_D0;
  
  *(undefined1 *)(in_D0 + 0x18) = 0;
  **(undefined1 **)(in_D0 + 0x14) = 0;
  return 0;
}



void FUN_00005100(void)

{
  ushort uVar1;
  short sVar2;
  undefined1 *in_D1;
  
  FUN_0000522e(1,*in_D1);
  uVar1 = *(ushort *)(in_D1 + 6);
  if (uVar1 < 0xfe) {
    FUN_0000522e(2,-(char)uVar1);
  }
  else {
    FUN_0000522e(2,0);
    FUN_0000522e(3,-(char)uVar1);
  }
  if (uVar1 < 0xfe) {
    sVar2 = 0x100 - uVar1;
  }
  else {
    sVar2 = 0x200 - uVar1;
  }
  FUN_00005308(sVar2,*(undefined4 *)(in_D1 + 2),uVar1);
  return;
}



void FUN_0000522e(undefined4 param_1,undefined1 param_2)

{
  int in_D0;
  
  FUN_000054ac(0);
  **(undefined1 **)(in_D0 + 0x2e) = param_2;
  return;
}



undefined1 FUN_00005268(void)

{
  int in_D0;
  
  FUN_000054ac(0x80);
  return **(undefined1 **)(in_D0 + 0x2e);
}



short FUN_000052a8(void)

{
  short sVar1;
  byte bVar2;
  
  bVar2 = FUN_00005268(2);
  if (bVar2 == 0) {
    bVar2 = FUN_00005268(3);
    sVar1 = 0x200 - (ushort)bVar2;
  }
  else {
    sVar1 = 0x100 - (ushort)bVar2;
  }
  return sVar1;
}



void FUN_00005308(undefined4 param_1,undefined1 *param_2,short param_3)

{
  int in_D0;
  
  FUN_000054ac(0x40);
  param_3 = param_3 + 1;
  while (param_3 = param_3 + -1, param_3 != 0) {
    **(undefined1 **)(in_D0 + 0x2e) = *param_2;
    param_2 = param_2 + 1;
  }
  return;
}



void FUN_00005356(undefined4 param_1,undefined1 *param_2,short param_3)

{
  int in_D0;
  
  FUN_000054ac(0xc0);
  param_3 = param_3 + 1;
  while (param_3 = param_3 + -1, param_3 != 0) {
    *param_2 = **(undefined1 **)(in_D0 + 0x2e);
    param_2 = param_2 + 1;
  }
  return;
}



int FUN_000053a4(void)

{
  int in_D0;
  int iVar1;
  int iVar2;
  
  iVar1 = 0;
  for (iVar2 = 0; (iVar1 < 2 && (*(char *)(in_D0 + 0x3a + iVar2) != '\x01')); iVar2 = iVar2 + 1) {
    iVar1 = iVar1 + 1;
  }
  if (iVar2 == 2) {
    iVar2 = -1;
  }
  else {
    *(undefined1 *)(in_D0 + 0x3a + iVar2) = 0;
  }
  return iVar2;
}



int FUN_000053f6(void)

{
  int in_D0;
  int iVar1;
  int iVar2;
  
  iVar1 = 0;
  for (iVar2 = 2; (iVar1 < 2 && (*(char *)(in_D0 + 0x3a + iVar2) != '\x01')); iVar2 = iVar2 + 1) {
    iVar1 = iVar1 + 1;
  }
  if (iVar1 == 2) {
    iVar2 = -1;
  }
  else {
    *(undefined1 *)(in_D0 + 0x3a + iVar2) = 0;
  }
  return iVar2;
}



undefined4 FUN_00005448(void)

{
  int in_D0;
  uint uVar1;
  
  uVar1 = CONCAT31((int3)((uint)in_D0 >> 8),**(undefined1 **)(in_D0 + 0x32)) & 0xfffffffc;
  **(byte **)(in_D0 + 0x32) = (byte)uVar1 | 1;
  return CONCAT31((int3)(uVar1 >> 8),**(undefined1 **)(in_D0 + 0x36));
}



void FUN_0000547c(void)

{
  int in_D0;
  undefined1 in_D1b;
  
  **(byte **)(in_D0 + 0x32) = **(byte **)(in_D0 + 0x32) & 0xfc | 1;
  **(undefined1 **)(in_D0 + 0x36) = in_D1b;
  return;
}



void FUN_000054ac(char param_1)

{
  int in_D0;
  undefined2 in_D1w;
  undefined1 uStack_9;
  
  **(char **)(in_D0 + 0x26) = param_1 + (char)((ushort)in_D1w >> 8);
  uStack_9 = (undefined1)in_D1w;
  **(undefined1 **)(in_D0 + 0x2a) = uStack_9;
  return;
}



void FUN_000054e0(void)

{
  int in_D0;
  
  **(byte **)(in_D0 + 0x32) = **(byte **)(in_D0 + 0x32) | 0x80;
  **(byte **)(in_D0 + 0x32) = **(byte **)(in_D0 + 0x32) & 0x7f;
  return;
}



int FUN_00005518(undefined4 param_1,int *param_2)

{
  int iVar1;
  uint in_D1;
  int unaff_A6;
  int iVar2;
  int iVar3;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  uint uStack_c;
  
  local_14 = -1;
  iVar3 = 0;
  iVar2 = 0;
  *(undefined4 *)(unaff_A6 + 0xa74) = 0;
  uStack_c = in_D1;
  FUN_000034fe(0,0);
  iVar1 = FUN_0000220c(1);
  if (iVar1 == 0) {
    FUN_00005a8a();
    FUN_000034fe();
    local_18 = FUN_000023e8();
    if (((local_18 < 1) || (local_1c = FUN_000057ea(&local_24), local_1c == 0)) ||
       (local_24 < local_18)) {
      *(undefined4 *)(unaff_A6 + 0xa74) = 0xcf;
    }
    else {
      if ('\0' < *(char *)(unaff_A6 + 0xb0c)) {
        FUN_0000583e(uStack_c & 0xff,local_18);
      }
      local_20 = local_1c;
      while ((iVar3 = FUN_000022c8(), 0 < iVar3 && (iVar3 == 0x10000))) {
        local_20 = local_20 + 0x10000;
        iVar2 = iVar2 + 0x10000;
      }
      if (iVar3 < 0) {
        *(undefined4 *)(unaff_A6 + 0xa74) = 0xf4;
      }
      else {
        local_14 = iVar3 + iVar2;
        *param_2 = local_1c;
      }
    }
    FUN_00002470();
  }
  else {
    *(undefined4 *)(unaff_A6 + 0xa74) = 0xd7;
  }
  if (*(int *)(unaff_A6 + 0xa74) == 0) {
    if ('\0' < *(char *)(unaff_A6 + 0xb0c)) {
      FUN_0000583e(local_20 + iVar3);
      FUN_00005ad8(0);
      FUN_0000583e();
    }
  }
  else {
    FUN_0000583e(uStack_c & 0xff,*(undefined4 *)(unaff_A6 + 0xa74));
  }
  return local_14;
}



void FUN_00005748(void)

{
  int iVar1;
  
  iVar1 = FUN_0000598a();
  *(byte *)(iVar1 + 0x12) = *(byte *)(iVar1 + 0x12) | 3;
  *(undefined1 *)(iVar1 + 0x10) = 0xbc;
  return;
}



void FUN_000057ea(undefined4 *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 *in_D1;
  short extraout_D1w;
  undefined1 uVar4;
  
  uVar1 = *in_D1;
  uVar4 = 0;
  iVar2 = FUN_000058ca();
  uVar3 = (*(code *)(*(int *)(iVar2 + 4) + (int)extraout_D1w))();
  if (!(bool)uVar4) {
    *in_D1 = uVar1;
    *param_1 = uVar3;
  }
  return;
}



void FUN_0000582a(void)

{
  FUN_000057ea();
  return;
}



void FUN_00005838(void)

{
  undefined4 in_D0;
  int unaff_A6;
  
  *(undefined4 *)(unaff_A6 + 0xb3c) = in_D0;
  return;
}



undefined8 FUN_0000583e(void)

{
  char cVar1;
  bool bVar2;
  char *in_D0;
  int iVar3;
  char *in_D1;
  short sVar4;
  undefined4 *extraout_A0;
  undefined4 *extraout_A0_00;
  undefined4 *puVar5;
  undefined4 *extraout_A0_01;
  int extraout_A1;
  int extraout_A1_00;
  char *pcVar6;
  char *pcVar7;
  
  bVar2 = true;
  iVar3 = FUN_000058ca();
  iVar3 = *(int *)(iVar3 + 4);
  puVar5 = extraout_A0;
  pcVar6 = in_D0;
  do {
    while( true ) {
      if (*pcVar6 == '\0') {
        return CONCAT44(in_D0,in_D1);
      }
      if (*pcVar6 == '%') break;
LAB_0000588c:
      sVar4 = 2;
      pcVar6 = pcVar6 + 1;
LAB_0000588e:
      (*(code *)(iVar3 + (short)(sVar4 << 2)))();
      puVar5 = extraout_A0_00;
      iVar3 = extraout_A1;
    }
    cVar1 = pcVar6[1];
    if (cVar1 == 'b') {
      sVar4 = 6;
LAB_00005896:
      pcVar6 = pcVar6 + 2;
      if (bVar2) {
        bVar2 = false;
      }
      goto LAB_0000588e;
    }
    if (cVar1 == 'w') {
      sVar4 = 7;
      goto LAB_00005896;
    }
    if (cVar1 == 'l') {
      sVar4 = 8;
      goto LAB_00005896;
    }
    if (cVar1 != 's') goto LAB_0000588c;
    pcVar6 = pcVar6 + 2;
    if (bVar2) {
      bVar2 = false;
      pcVar7 = in_D1;
    }
    else {
      pcVar7 = (char *)*puVar5;
      puVar5 = puVar5 + 1;
    }
    while (*pcVar7 != '\0') {
      (*(code *)(iVar3 + 8))();
      puVar5 = extraout_A0_01;
      iVar3 = extraout_A1_00;
      pcVar7 = pcVar7 + 1;
    }
  } while( true );
}



undefined4 FUN_000058ca(void)

{
  return 0;
}



undefined4 FUN_000058d2(void)

{
  return 0x10000;
}



undefined4 FUN_000058fa(void)

{
  return 0x1200000;
}



undefined4 FUN_0000590a(void)

{
  return 0x600000;
}



undefined4 FUN_0000591a(void)

{
  return 0x7fffe0;
}



undefined4 FUN_00005922(void)

{
  return 0xfc00;
}



undefined4 FUN_0000596a(void)

{
  return 0xd00000;
}



undefined4 FUN_0000598a(void)

{
  return 0xc00000;
}



undefined4 FUN_00005992(void)

{
  return 0xe00000;
}



undefined4 FUN_0000599a(void)

{
  return 0xf00000;
}



undefined1 * FUN_000059b2(void)

{
  return &DAT_00802400;
}



undefined4 FUN_000059ca(void)

{
  return 0x9ffc00;
}



undefined4 FUN_000059d2(void)

{
  return 0xa00000;
}



undefined4 FUN_000059da(void)

{
  return 0x9f0000;
}



undefined4 FUN_000059e2(void)

{
  return 0x9ff000;
}



undefined4 FUN_00005a1a(void)

{
  return 0xd00400;
}



undefined4 FUN_00005a3a(void)

{
  return 0xd01ff0;
}



undefined4 FUN_00005a4a(void)

{
  return 0x802000;
}



undefined4 FUN_00005a5a(void)

{
  return 0x320000;
}



undefined4 FUN_00005a6a(void)

{
  return 0xe0000;
}



undefined4 FUN_00005a7a(void)

{
  return 0x1e0000;
}



void FUN_00005a8a(void)

{
  int in_D0;
  uint uVar1;
  uint uVar2;
  ushort uVar3;
  
  uVar1 = in_D0 * 1000 - 0x1ab6;
  if ((uint)(in_D0 * 1000) < 0x1ab6 || uVar1 == 0) {
    uVar1 = 0;
  }
  uVar1 = uVar1 / 0xfa;
  uVar2 = uVar1 >> 0x10;
  while( true ) {
    do {
      uVar3 = (short)uVar1 - 1;
      uVar1 = (uint)uVar3;
    } while (uVar3 != 0xffff);
    uVar3 = (short)uVar2 - 1;
    uVar2 = (uint)uVar3;
    if (uVar3 == 0xffff) break;
    uVar1 = 0xffff;
  }
  return;
}



int FUN_00005ad8(byte *param_1)

{
  byte *in_D0;
  int in_D1;
  int iVar1;
  int iVar2;
  byte *pbVar3;
  
  iVar2 = 0;
  pbVar3 = in_D0;
  while (in_D1 != 0) {
    iVar2 = (uint)*pbVar3 + iVar2;
    in_D1 = in_D1 + -1;
    pbVar3 = pbVar3 + 1;
  }
  if (((param_1 != (byte *)0x0) && (param_1 < pbVar3)) && (in_D0 <= param_1)) {
    iVar1 = 4;
    while (iVar1 != 0) {
      iVar2 = iVar2 - (uint)*param_1;
      iVar1 = iVar1 + -1;
      param_1 = param_1 + 1;
    }
  }
  return iVar2;
}



byte FUN_00005b2c(void)

{
  char in_XF;
  char in_NF;
  char in_ZF;
  char in_VF;
  byte in_CF;
  
  return in_XF << 4 | in_NF << 3 | in_ZF << 2 | in_VF << 1 | in_CF;
}



undefined8 FUN_00005bb4(void)

{
  int in_D0;
  undefined4 in_D1;
  char in_XF;
  
  return CONCAT44(CONCAT22((short)((uint)in_D0 >> 0x10),
                           (ushort)(byte)(in_XF << 4 | (in_D0 < 0) << 3 | (in_D0 == 0) << 2)),in_D1)
  ;
}



ulonglong FUN_00005bc0(void)

{
  byte bVar1;
  byte bVar2;
  ushort *in_D0;
  uint uVar3;
  ushort uVar4;
  ushort *in_D1;
  ushort *puVar5;
  ushort *puVar6;
  
  if ((((byte)in_D1 ^ (byte)in_D0) & 1) != 0) {
    uVar3 = 0;
    do {
      bVar1 = *(byte *)in_D0;
      uVar4 = (ushort)CONCAT31((int3)(uVar3 >> 8),bVar1);
      puVar6 = (ushort *)((int)in_D1 + 1);
      bVar2 = *(byte *)in_D1;
      if (bVar1 != bVar2) break;
      uVar4 = uVar4 - 1;
      uVar3 = (uint)uVar4;
      in_D0 = (ushort *)((int)in_D0 + 1);
      in_D1 = puVar6;
    } while (uVar4 != 0xffff);
    if (bVar2 <= bVar1) {
      return (ulonglong)CONCAT24(uVar4 + 1,puVar6);
    }
LAB_00005c08:
    return CONCAT44(0xffffffff,puVar6);
  }
  puVar5 = in_D0;
  if (((uint)in_D1 & 1) != 0) {
    puVar6 = (ushort *)((int)in_D1 + 1);
    if (*(byte *)in_D0 < *(byte *)in_D1) goto LAB_00005c08;
    if (*(byte *)in_D1 < *(byte *)in_D0) {
LAB_00005bfa:
      return CONCAT44(1,puVar6);
    }
    puVar5 = (ushort *)((int)in_D0 + 1);
    in_D1 = puVar6;
    if (*(char *)in_D0 == '\0') goto LAB_00005c16;
  }
  while( true ) {
    uVar4 = *puVar5;
    puVar6 = in_D1 + 1;
    if (uVar4 != *in_D1) break;
    if (((char)uVar4 == '\0') || (puVar5 = puVar5 + 1, in_D1 = puVar6, uVar4 < 0x100))
    goto LAB_00005c16;
  }
  if (uVar4 < *in_D1) {
    if (0xff < uVar4) goto LAB_00005c08;
  }
  else if (0xff < uVar4) goto LAB_00005bfa;
  if (*(char *)in_D1 != '\0') goto LAB_00005c08;
LAB_00005c16:
  return ZEXT48(puVar6);
}



char * FUN_00005c34(void)

{
  char *in_D0;
  char *pcVar1;
  char *pcVar2;
  char *pcVar3;
  
  pcVar2 = in_D0;
  do {
    pcVar3 = pcVar2 + 1;
    if (((*pcVar2 == '\0') || (pcVar3 = pcVar2 + 2, pcVar2[1] == '\0')) ||
       (pcVar1 = pcVar2 + 3, pcVar3 = pcVar1, pcVar2[2] == '\0')) break;
    pcVar2 = pcVar2 + 4;
    pcVar3 = pcVar2;
  } while (*pcVar1 != '\0');
  return pcVar3 + (-1 - (int)in_D0);
}



void FUN_00005c52(void)

{
  char cVar1;
  char *in_D0;
  char *in_D1;
  char *pcVar2;
  char *pcVar3;
  
  do {
    cVar1 = *in_D1;
    *in_D0 = cVar1;
    if (cVar1 == '\0') {
      return;
    }
    cVar1 = in_D1[1];
    in_D0[1] = cVar1;
    if (cVar1 == '\0') {
      return;
    }
    pcVar3 = in_D1 + 3;
    cVar1 = in_D1[2];
    pcVar2 = in_D0 + 3;
    in_D0[2] = cVar1;
    if (cVar1 == '\0') {
      return;
    }
    in_D1 = in_D1 + 4;
    cVar1 = *pcVar3;
    in_D0 = in_D0 + 4;
    *pcVar2 = cVar1;
  } while (cVar1 != '\0');
  return;
}



void FUN_00005c6e(void)

{
  char cVar1;
  char *in_D0;
  char *in_D1;
  char *pcVar2;
  char *pcVar3;
  
  do {
    pcVar2 = in_D0 + 1;
    if (((*in_D0 == '\0') || (pcVar2 = in_D0 + 2, in_D0[1] == '\0')) ||
       (pcVar3 = in_D0 + 3, pcVar2 = pcVar3, in_D0[2] == '\0')) break;
    in_D0 = in_D0 + 4;
    pcVar2 = in_D0;
  } while (*pcVar3 != '\0');
  cVar1 = *in_D1;
  pcVar2[-1] = cVar1;
  while( true ) {
    if (cVar1 == '\0') {
      return;
    }
    cVar1 = in_D1[1];
    *pcVar2 = cVar1;
    if (cVar1 == '\0') {
      return;
    }
    pcVar3 = in_D1 + 3;
    cVar1 = in_D1[2];
    pcVar2[1] = cVar1;
    if (cVar1 == '\0') {
      return;
    }
    in_D1 = in_D1 + 4;
    cVar1 = *pcVar3;
    pcVar3 = pcVar2 + 3;
    pcVar2[2] = cVar1;
    if (cVar1 == '\0') break;
    cVar1 = *in_D1;
    pcVar2 = pcVar2 + 4;
    *pcVar3 = cVar1;
  }
  return;
}



int FUN_00005ca2(int param_1)

{
  char *in_D0;
  char *in_D1;
  
  while( true ) {
    if (param_1 == 0) {
      return 0;
    }
    if (*in_D0 != *in_D1) break;
    in_D0 = in_D0 + 1;
    in_D1 = in_D1 + 1;
    param_1 = param_1 + -1;
  }
  return (int)(short)((short)*in_D0 - (short)*in_D1);
}



undefined8 FUN_00005cfa(int param_1)

{
  undefined4 in_D0;
  undefined4 in_D1;
  
  if (param_1 != 0) {
    FUN_00005d18();
  }
  return CONCAT44(in_D0,in_D1);
}



void FUN_00005d18(void)

{
  uint unaff_D2;
  uint uVar1;
  uint uVar2;
  undefined4 *in_A0;
  undefined4 *puVar4;
  undefined1 *extraout_A0;
  int extraout_A0_00;
  undefined4 *unaff_A2;
  undefined4 *puVar5;
  short sVar3;
  
  if (unaff_D2 != 0) {
    if (unaff_A2 < in_A0) {
      puVar5 = (undefined4 *)(unaff_D2 + (int)unaff_A2);
      puVar4 = (undefined4 *)(unaff_D2 + (int)in_A0);
      if (((uint)puVar5 & 1) != 0) {
        puVar5 = (undefined4 *)((int)puVar5 + -1);
        puVar4 = (undefined4 *)((int)puVar4 + -1);
        *(undefined1 *)puVar4 = *(undefined1 *)puVar5;
        unaff_D2 = unaff_D2 - 1;
      }
      if (((uint)puVar4 & 1) == 0) {
        if ((unaff_D2 & 1) != 0) {
          FUN_00005d8a();
          *(undefined1 *)(extraout_A0_00 + -1) = *(undefined1 *)((int)puVar5 + -1);
          return;
        }
        uVar2 = unaff_D2 >> 2;
        if ((unaff_D2 >> 1 & 1) != 0) {
          puVar5 = (undefined4 *)((int)puVar5 + -2);
          puVar4 = (undefined4 *)((int)puVar4 + -2);
          *(undefined2 *)puVar4 = *(undefined2 *)puVar5;
        }
        while ((sVar3 = (short)uVar2 + -1, uVar1 = CONCAT22((short)(uVar2 >> 0x10),sVar3),
               sVar3 != -1 || (uVar1 = uVar2 - 1, uVar2 != 0))) {
          puVar5 = puVar5 + -1;
          puVar4 = puVar4 + -1;
          *puVar4 = *puVar5;
          uVar2 = uVar1;
        }
        return;
      }
      while ((sVar3 = (short)unaff_D2 + -1, uVar2 = CONCAT22((short)(unaff_D2 >> 0x10),sVar3),
             sVar3 != -1 || (uVar2 = unaff_D2 - 1, unaff_D2 != 0))) {
        puVar5 = (undefined4 *)((int)puVar5 + -1);
        puVar4 = (undefined4 *)((int)puVar4 + -1);
        *(undefined1 *)puVar4 = *(undefined1 *)puVar5;
        unaff_D2 = uVar2;
      }
      return;
    }
    if (in_A0 != unaff_A2) {
      puVar4 = in_A0;
      puVar5 = unaff_A2;
      if (((uint)unaff_A2 & 1) != 0) {
        puVar5 = (undefined4 *)((int)unaff_A2 + 1);
        puVar4 = (undefined4 *)((int)in_A0 + 1);
        *(undefined1 *)in_A0 = *(undefined1 *)unaff_A2;
        unaff_D2 = unaff_D2 - 1;
      }
      if (((uint)puVar4 & 1) == 0) {
        if ((unaff_D2 & 1) != 0) {
          FUN_00005d40();
          *extraout_A0 = *(undefined1 *)puVar5;
          return;
        }
        uVar2 = unaff_D2 >> 2;
        if ((unaff_D2 >> 1 & 1) != 0) {
          *(undefined2 *)puVar4 = *(undefined2 *)puVar5;
          puVar4 = (undefined4 *)((int)puVar4 + 2);
          puVar5 = (undefined4 *)((int)puVar5 + 2);
        }
        while ((sVar3 = (short)uVar2 + -1, uVar1 = CONCAT22((short)(uVar2 >> 0x10),sVar3),
               sVar3 != -1 || (uVar1 = uVar2 - 1, uVar2 != 0))) {
          *puVar4 = *puVar5;
          uVar2 = uVar1;
          puVar4 = puVar4 + 1;
          puVar5 = puVar5 + 1;
        }
        return;
      }
      while ((sVar3 = (short)unaff_D2 + -1, uVar2 = CONCAT22((short)(unaff_D2 >> 0x10),sVar3),
             sVar3 != -1 || (uVar2 = unaff_D2 - 1, unaff_D2 != 0))) {
        *(undefined1 *)puVar4 = *(undefined1 *)puVar5;
        unaff_D2 = uVar2;
        puVar4 = (undefined4 *)((int)puVar4 + 1);
        puVar5 = (undefined4 *)((int)puVar5 + 1);
      }
    }
  }
  return;
}



void FUN_00005d40(void)

{
  uint unaff_D2;
  uint uVar1;
  undefined4 *in_A0;
  undefined4 *unaff_A2;
  uint uVar2;
  short sVar3;
  
  uVar2 = unaff_D2 >> 1;
  if ((unaff_D2 & 1) != 0) {
    *(undefined2 *)in_A0 = *(undefined2 *)unaff_A2;
    in_A0 = (undefined4 *)((int)in_A0 + 2);
    unaff_A2 = (undefined4 *)((int)unaff_A2 + 2);
  }
  while ((sVar3 = (short)uVar2 + -1, uVar1 = CONCAT22((short)(uVar2 >> 0x10),sVar3), sVar3 != -1 ||
         (uVar1 = uVar2 - 1, uVar2 != 0))) {
    *in_A0 = *unaff_A2;
    uVar2 = uVar1;
    in_A0 = in_A0 + 1;
    unaff_A2 = unaff_A2 + 1;
  }
  return;
}



void FUN_00005d8a(void)

{
  uint unaff_D2;
  uint uVar1;
  undefined4 *in_A0;
  undefined4 *unaff_A2;
  uint uVar2;
  short sVar3;
  
  uVar2 = unaff_D2 >> 1;
  if ((unaff_D2 & 1) != 0) {
    unaff_A2 = (undefined4 *)((int)unaff_A2 + -2);
    in_A0 = (undefined4 *)((int)in_A0 + -2);
    *(undefined2 *)in_A0 = *(undefined2 *)unaff_A2;
  }
  while ((sVar3 = (short)uVar2 + -1, uVar1 = CONCAT22((short)(uVar2 >> 0x10),sVar3), sVar3 != -1 ||
         (uVar1 = uVar2 - 1, uVar2 != 0))) {
    unaff_A2 = unaff_A2 + -1;
    in_A0 = in_A0 + -1;
    *in_A0 = *unaff_A2;
    uVar2 = uVar1;
  }
  return;
}



undefined4 * FUN_00005db2(uint param_1)

{
  undefined2 uVar1;
  int iVar2;
  undefined4 *in_D0;
  uint uVar3;
  int iVar4;
  ushort uVar5;
  undefined1 in_D1b;
  undefined4 uVar6;
  short sVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  
  if (param_1 < 0xc) {
    sVar7 = (short)param_1 + -1;
    puVar8 = in_D0;
    if ((short)param_1 == 0) {
      return in_D0;
    }
    do {
      *(undefined1 *)puVar8 = in_D1b;
      sVar7 = sVar7 + -1;
      puVar8 = (undefined4 *)((int)puVar8 + 1);
    } while (sVar7 != -1);
    return in_D0;
  }
  uVar1 = CONCAT11(in_D1b,in_D1b);
  puVar8 = in_D0;
  if (((uint)in_D0 & 1) != 0) {
    puVar8 = (undefined4 *)((int)in_D0 + 1);
    *(undefined1 *)in_D0 = in_D1b;
    param_1 = param_1 - 1;
  }
  puVar9 = puVar8;
  if (((uint)puVar8 & 2) != 0) {
    puVar9 = (undefined4 *)((int)puVar8 + 2);
    *(undefined2 *)puVar8 = uVar1;
    param_1 = param_1 - 2;
  }
  uVar6 = CONCAT22(uVar1,uVar1);
  uVar5 = (ushort)param_1;
  if (param_1 < 0x60) {
    uVar5 = uVar5 >> 2;
  }
  else {
    uVar3 = param_1 >> 0x10;
    param_1 = param_1 & 0xffff001f;
    uVar3 = CONCAT22((short)uVar3,(ushort)param_1 ^ uVar5);
    puVar9 = (undefined4 *)((int)puVar9 + uVar3);
    iVar2 = (uVar3 >> 5) - 1;
    puVar8 = puVar9;
    do {
      do {
        iVar4 = iVar2;
        puVar8[-1] = uVar6;
        puVar8[-2] = uVar6;
        puVar8[-3] = uVar6;
        puVar8[-4] = uVar6;
        puVar8[-5] = uVar6;
        puVar8[-6] = uVar6;
        puVar8[-7] = uVar6;
        puVar8 = puVar8 + -8;
        *puVar8 = uVar6;
        sVar7 = (short)iVar4 + -1;
        iVar2 = CONCAT22((short)((uint)iVar4 >> 0x10),sVar7);
      } while (sVar7 != -1);
      iVar2 = iVar4 + -1;
    } while (iVar4 != 0);
    if ((ushort)param_1 == 0) {
      return in_D0;
    }
    uVar5 = (uVar5 & 0x1f) >> 2;
    if (uVar5 == 0) goto LAB_00005e4e;
  }
  sVar7 = uVar5 - 1;
  puVar8 = puVar9;
  do {
    puVar9 = puVar8 + 1;
    *puVar8 = uVar6;
    sVar7 = sVar7 + -1;
    puVar8 = puVar9;
  } while (sVar7 != -1);
LAB_00005e4e:
  puVar8 = puVar9;
  if ((param_1 & 2) != 0) {
    puVar8 = (undefined4 *)((int)puVar9 + 2);
    *(undefined2 *)puVar9 = uVar1;
  }
  if ((param_1 & 1) != 0) {
    *(undefined1 *)puVar8 = in_D1b;
  }
  return in_D0;
}



// WARNING: This function may have set the stack pointer
// WARNING: Removing unreachable block (ram,0x00005f10)
// WARNING: Removing unreachable block (ram,0x00005f22)
// WARNING: Removing unreachable block (ram,0x00005f32)
// WARNING: Removing unreachable block (ram,0x00005f38)
// WARNING: Removing unreachable block (ram,0x00005f48)
// WARNING: Removing unreachable block (ram,0x00005f4e)
// WARNING: Removing unreachable block (ram,0x00005f5a)
// WARNING: Removing unreachable block (ram,0x00005f6a)
// WARNING: Removing unreachable block (ram,0x00005f70)
// WARNING: Removing unreachable block (ram,0x00005f7c)
// WARNING: Removing unreachable block (ram,0x00005f8c)
// WARNING: Removing unreachable block (ram,0x00005fd0)
// WARNING: Removing unreachable block (ram,0x00005fee)
// WARNING: Removing unreachable block (ram,0x00006014)
// WARNING: Removing unreachable block (ram,0x00006022)
// WARNING: Removing unreachable block (ram,0x00006074)
// WARNING: Removing unreachable block (ram,0x0000608a)
// WARNING: Removing unreachable block (ram,0x000060a6)
// WARNING: Removing unreachable block (ram,0x000060d0)
// WARNING: Removing unreachable block (ram,0x000060d8)
// WARNING: Removing unreachable block (ram,0x000060de)
// WARNING: Removing unreachable block (ram,0x000060f8)
// WARNING: Removing unreachable block (ram,0x0000607e)
// WARNING: Removing unreachable block (ram,0x00005f8e)
// WARNING: Removing unreachable block (ram,0x00005eee)
// WARNING: Removing unreachable block (ram,0x00005ef8)
// WARNING: Removing unreachable block (ram,0x00006026)

void FUN_00005e68(void)

{
                    // WARNING: Do nothing block with infinite loop
                    // WARNING: Could not recover jumptable at 0x00006088. Too many branches
                    // WARNING: Treating indirect jump as call
  do {
  } while( true );
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0000608e(void)

{
  bool bVar1;
  int iVar2;
  uint uVar3;
  
  FUN_00006308();
  do {
    iVar2 = 1;
    do {
      bVar1 = -1 < iVar2;
      iVar2 = iVar2 << 1;
    } while (bVar1);
    uVar3 = 1;
    do {
      *(undefined1 *)(uVar3 | 0x800000) = 0;
      uVar3 = uVar3 * 2;
    } while (uVar3 != 0x200000);
  } while( true );
}



void FUN_00006198(void)

{
  undefined1 in_D1b;
  code *UNRECOVERED_JUMPTABLE;
  int unaff_A3;
  int unaff_A6;
  
  do {
    if (*(char *)(unaff_A6 + 0xb47) == '\0') goto LAB_000061ac;
  } while ((*(byte *)(unaff_A3 + 1) & 4) == 0);
  *(undefined1 *)(unaff_A3 + 3) = in_D1b;
LAB_000061ac:
                    // WARNING: Could not recover jumptable at 0x000061ac. Too many branches
                    // WARNING: Treating indirect jump as call
  (*UNRECOVERED_JUMPTABLE)();
  return;
}



void FUN_000061ae(void)

{
  FUN_00006198();
  return;
}



void FUN_000062b0(void)

{
  DAT_00e00008 = 7;
  DAT_00e00009 = 0xbb;
  DAT_00e0000a = 0x40;
  return;
}



void FUN_00006308(void)

{
  int unaff_A6;
  
  FUN_00006fae();
  FUN_0000886a(&DAT_0000637c);
  FUN_0000637e();
  FUN_00006b3a();
  FUN_00007b50();
  FUN_000070c2();
  FUN_000070ae();
  FUN_00007ce4();
  FUN_000088ec();
  *(undefined1 *)(unaff_A6 + 0xb64) = 0;
  *(undefined1 *)(unaff_A6 + 0xb66) = 0;
  do {
                    // WARNING: Do nothing block with infinite loop
  } while( true );
}



void FUN_0000637e(void)

{
  int in_D0;
  int unaff_A6;
  
  *(undefined2 *)(unaff_A6 + 0xb42) = 0;
  FUN_000063ac();
  if (in_D0 != 0) {
    FUN_000065ee();
    if (in_D0 != 0) {
      FUN_00006806();
    }
  }
  return;
}



void FUN_000063ac(void)

{
  undefined1 *in_D0;
  int unaff_A6;
  int local_14;
  
  *in_D0 = 1;
  FUN_000083f2();
  if (local_14 != 0) {
    switch(local_14) {
    case 1:
      FUN_0000886a(0);
      break;
    case 2:
      FUN_0000886a(0);
      *in_D0 = 0;
      break;
    case 3:
      FUN_0000886a(0);
      *in_D0 = 0;
      break;
    case 4:
      FUN_0000886a(0);
      *in_D0 = 0;
      break;
    case 5:
      FUN_0000886a(0);
      *in_D0 = 0;
      break;
    case 6:
      FUN_0000886a(0);
      *in_D0 = 0;
      break;
    case 7:
      FUN_0000886a(0);
      *in_D0 = 0;
      break;
    case 8:
      FUN_0000886a(0);
      *in_D0 = 0;
      break;
    case 9:
      FUN_0000886a(0);
      *in_D0 = 0;
      break;
    case 10:
      FUN_0000886a(0);
      *in_D0 = 0;
      break;
    case 0xb:
      FUN_0000886a(0);
      *in_D0 = 0;
      break;
    case 0xc:
      FUN_0000886a(0);
      *in_D0 = 0;
      break;
    case 0xd:
      FUN_0000886a(0);
      *in_D0 = 0;
      break;
    case 0xe:
      FUN_0000886a(0);
      *in_D0 = 0;
      break;
    case 0xf:
      FUN_0000886a(0);
      *in_D0 = 0;
      break;
    case 0x10:
      FUN_0000886a(0);
      *in_D0 = 0;
      break;
    case 0x11:
      FUN_0000886a(0);
      *in_D0 = 0;
      break;
    default:
      FUN_0000886a(0);
      *in_D0 = 0;
    }
    *(short *)(unaff_A6 + 0xb42) = *(short *)(unaff_A6 + 0xb42) + 1;
  }
  return;
}



void FUN_000065ee(void)

{
  char *in_D0;
  int unaff_A6;
  int local_14;
  
  *in_D0 = '\x01';
  if ((*in_D0 != '\0') && (FUN_00007dc6(), local_14 != 0)) {
    switch(local_14) {
    case 1:
      FUN_0000886a(0);
      break;
    case 2:
      FUN_0000886a(0);
      break;
    case 3:
      FUN_0000886a(0);
      break;
    case 4:
      FUN_0000886a(0);
      break;
    case 5:
      FUN_0000886a(0);
      break;
    case 6:
      FUN_0000886a(0);
      break;
    case 7:
      FUN_0000886a(0);
      break;
    case 8:
      FUN_0000886a(0);
      break;
    case 9:
      FUN_0000886a(0);
      break;
    case 10:
      FUN_0000886a(0);
      break;
    case 0xb:
      FUN_0000886a(0);
      break;
    case 0xc:
      FUN_0000886a(0);
      break;
    case 0xd:
      FUN_0000886a(0);
      break;
    case 0xe:
      FUN_0000886a(0);
      break;
    case 0xf:
      FUN_0000886a(0);
      break;
    case 0x10:
      FUN_0000886a(0);
      break;
    case 0x11:
      FUN_0000886a(0);
      break;
    case 0x12:
      FUN_0000886a(0);
      break;
    case 0x13:
      FUN_0000886a(0);
      break;
    default:
      FUN_0000886a(0);
    }
    *(short *)(unaff_A6 + 0xb42) = *(short *)(unaff_A6 + 0xb42) + 1;
  }
  return;
}



void FUN_00006806(void)

{
  undefined1 *in_D0;
  undefined1 *in_D1;
  int unaff_A6;
  int local_14;
  
  *in_D0 = 1;
  FUN_0000704e();
  if (local_14 == 0) {
    *in_D1 = 0;
  }
  else {
    switch(local_14) {
    case 1:
      FUN_0000886a(0);
      break;
    case 2:
      FUN_0000886a(0);
      break;
    case 3:
      FUN_0000886a(0);
      break;
    case 4:
      FUN_0000886a(0);
      break;
    case 5:
      FUN_0000886a(0);
      break;
    case 6:
      FUN_0000886a(0);
      break;
    case 7:
      FUN_0000886a(0);
      break;
    case 8:
      FUN_0000886a(0);
      break;
    case 9:
      FUN_0000886a(0);
      break;
    case 10:
      FUN_0000886a(0);
      break;
    case 0xb:
      FUN_0000886a(0);
      break;
    case 0xc:
      FUN_0000886a(0);
      break;
    case 0xd:
      FUN_0000886a(0);
      break;
    case 0xe:
      FUN_0000886a(0);
      break;
    case 0xf:
      FUN_0000886a(0);
      break;
    case 0x10:
      FUN_0000886a(0);
      break;
    case 0x11:
      FUN_0000886a(0);
      break;
    case 0x12:
      FUN_0000886a(0);
      break;
    case 0x13:
      FUN_0000886a(0);
      break;
    case 0x14:
      FUN_0000886a(0);
      break;
    case 0x15:
      FUN_0000886a(0);
      break;
    case 0x16:
      FUN_0000886a(0);
      break;
    case 0x17:
      FUN_0000886a(0);
      break;
    case 0x18:
      FUN_0000886a(0);
      break;
    case 0x19:
      FUN_0000886a(0);
      break;
    case 0x1a:
      FUN_0000886a(0);
      break;
    case 0x1b:
      FUN_0000886a(0);
      break;
    case 0x1c:
      FUN_0000886a(0);
      break;
    case 0x1d:
      FUN_0000886a(0);
      break;
    case 0x1e:
      FUN_0000886a(0);
      break;
    case 0x1f:
      FUN_0000886a(0);
      break;
    case 0x20:
      FUN_0000886a(0);
      break;
    default:
      FUN_0000886a(0);
    }
    *in_D1 = 1;
    *(short *)(unaff_A6 + 0xb42) = *(short *)(unaff_A6 + 0xb42) + 1;
  }
  return;
}



void FUN_00006b3a(void)

{
  int in_D0;
  int unaff_A6;
  
  *(undefined2 *)(unaff_A6 + 0xb44) = 0;
  FUN_00006b5c();
  if (in_D0 != 0) {
    FUN_00006b6e();
  }
  return;
}



void FUN_00006b5c(void)

{
  FUN_00006baa();
  return;
}



void FUN_00006b6e(void)

{
  char *in_D0;
  
  FUN_00006cc2();
  if ((*in_D0 != '\0') && (FUN_00006c24(), *in_D0 != '\0')) {
    FUN_00006f3a();
  }
  return;
}



void FUN_00006baa(void)

{
  undefined1 *in_D0;
  int unaff_A6;
  int local_14;
  
  *in_D0 = 1;
  FUN_000083ac();
  if (local_14 != 0) {
    if (local_14 == 1) {
      FUN_0000886a(0);
      *(short *)(unaff_A6 + 0xb44) = *(short *)(unaff_A6 + 0xb44) + 1;
      *in_D0 = 0;
    }
    else {
      FUN_0000886a(0);
    }
  }
  return;
}



void FUN_00006c24(void)

{
  undefined1 *in_D0;
  int unaff_A6;
  undefined1 auStack_1c [8];
  int local_14;
  undefined1 *local_10;
  
  *in_D0 = 1;
  local_10 = in_D0;
  FUN_00007dd8(auStack_1c,&local_14);
  if (local_14 == 0) {
    FUN_0000886a(0);
  }
  else if (local_14 == 0x14) {
    FUN_0000886a(0);
    *(short *)(unaff_A6 + 0xb44) = *(short *)(unaff_A6 + 0xb44) + 1;
    *local_10 = 0;
  }
  else {
    FUN_0000886a(0);
  }
  return;
}



void FUN_00006cc2(void)

{
  undefined1 *in_D0;
  undefined4 local_14;
  
  *in_D0 = 1;
  FUN_00007f2c();
  switch(local_14) {
  case 0:
    break;
  case 1:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 2:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 3:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 4:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 5:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 6:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 7:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 8:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 9:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 10:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 0xb:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 0xc:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 0xd:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 0xe:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 0xf:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 0x10:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 0x11:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 0x12:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  case 0x13:
    FUN_0000886a(0);
    *in_D0 = 0;
    break;
  default:
    FUN_0000886a(0);
  }
  return;
}



void FUN_00006f3a(void)

{
  undefined1 *in_D0;
  int unaff_A6;
  int local_14;
  
  *in_D0 = 1;
  FUN_00007ece();
  if (local_14 != 0) {
    if (local_14 == 0x15) {
      FUN_0000886a(0);
      *(short *)(unaff_A6 + 0xb44) = *(short *)(unaff_A6 + 0xb44) + 1;
    }
    else {
      FUN_0000886a(0);
    }
  }
  return;
}



undefined1 FUN_00006fae(void)

{
  int unaff_A6;
  
  *(undefined1 *)(unaff_A6 + 0xb47) = 1;
  FUN_00007b7e();
  return *(undefined1 *)(unaff_A6 + 0xb47);
}



void FUN_0000704e(void)

{
  int *in_D0;
  
  FUN_00007882();
  if (*in_D0 == 0) {
    FUN_00007882();
  }
  return;
}



void FUN_00007080(void)

{
  undefined1 *in_D0;
  
  *in_D0 = DAT_00c0001c;
  return;
}



void FUN_0000709a(void)

{
  undefined1 in_D0b;
  
  DAT_00c0001c = in_D0b;
  return;
}



void FUN_000070ae(void)

{
  undefined1 in_D0b;
  
  DAT_009f100d = in_D0b;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_000070c2(void)

{
  undefined4 in_D0;
  
  _DAT_009f1000 = in_D0;
  return;
}



void FUN_00007882(void)

{
  bool bVar1;
  uint in_D0;
  undefined4 *in_D1;
  ushort local_20;
  ushort local_1e;
  uint local_1c;
  uint local_18;
  undefined1 local_14;
  
  *in_D1 = 0;
  local_14 = (undefined1)(in_D0 >> 0x18);
  DAT_00c00001 = local_14;
  FUN_00007b3e();
  FUN_00007b64();
  local_18 = CONCAT13(DAT_00c00001,local_18._1_3_);
  local_1c = local_18 ^ in_D0;
  if (local_1c != 0) {
    local_1e = 0;
    bVar1 = false;
    while ((local_1e < 0x20 && (!bVar1))) {
      if ((local_1c & 1) != 0) {
        local_20 = local_1e;
        bVar1 = true;
      }
      local_1e = local_1e + 1;
      local_1c = local_1c >> 1;
    }
    switch(local_20) {
    case 0:
      *in_D1 = 1;
      break;
    case 1:
      *in_D1 = 2;
      break;
    case 2:
      *in_D1 = 3;
      break;
    case 3:
      *in_D1 = 4;
      break;
    case 4:
      *in_D1 = 5;
      break;
    case 5:
      *in_D1 = 6;
      break;
    case 6:
      *in_D1 = 7;
      break;
    case 7:
      *in_D1 = 8;
      break;
    case 8:
      *in_D1 = 9;
      break;
    case 9:
      *in_D1 = 10;
      break;
    case 10:
      *in_D1 = 0xb;
      break;
    case 0xb:
      *in_D1 = 0xc;
      break;
    case 0xc:
      *in_D1 = 0xd;
      break;
    case 0xd:
      *in_D1 = 0xe;
      break;
    case 0xe:
      *in_D1 = 0xf;
      break;
    case 0xf:
      *in_D1 = 0x10;
      break;
    case 0x10:
      *in_D1 = 0x11;
      break;
    case 0x11:
      *in_D1 = 0x12;
      break;
    case 0x12:
      *in_D1 = 0x13;
      break;
    case 0x13:
      *in_D1 = 0x14;
      break;
    case 0x14:
      *in_D1 = 0x15;
      break;
    case 0x15:
      *in_D1 = 0x16;
      break;
    case 0x16:
      *in_D1 = 0x17;
      break;
    case 0x17:
      *in_D1 = 0x18;
      break;
    case 0x18:
      *in_D1 = 0x19;
      break;
    case 0x19:
      *in_D1 = 0x1a;
      break;
    case 0x1a:
      *in_D1 = 0x1b;
      break;
    case 0x1b:
      *in_D1 = 0x1c;
      break;
    case 0x1c:
      *in_D1 = 0x1d;
      break;
    case 0x1d:
      *in_D1 = 0x1e;
      break;
    case 0x1e:
      *in_D1 = 0x1f;
      break;
    case 0x1f:
      *in_D1 = 0x20;
    }
  }
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_00007b3e(void)

{
  undefined4 in_D0;
  
  _DAT_00c00004 = in_D0;
  return;
}



void FUN_00007b50(void)

{
  undefined1 in_D0b;
  
  DAT_00c00028 = in_D0b;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_00007b64(void)

{
  undefined4 *in_D0;
  
  *in_D0 = _DAT_00c00004;
  return;
}



void FUN_00007b7e(void)

{
  uint in_D0;
  uint uVar1;
  
  for (uVar1 = 0; uVar1 < in_D0; uVar1 = uVar1 + 1) {
  }
  return;
}



void FUN_00007ce4(void)

{
  FUN_00007080();
  FUN_0000709a();
  return;
}



void FUN_00007dc6(void)

{
  FUN_00007f2c();
  return;
}



void FUN_00007dd8(undefined4 param_1,int *param_2)

{
  undefined4 in_D0;
  undefined4 *in_D1;
  
  FUN_00008184(0x55555555);
  FUN_000081d2(0x55555555,in_D0,param_1,param_2);
  if (*param_2 == 0) {
    FUN_00008184(0xaaaaaaaa);
    FUN_000081d2(0xaaaaaaaa,in_D0,param_1,param_2);
    if ((*param_2 != 0) && (*param_2 == 0x14)) {
      *in_D1 = 0xaaaaaaaa;
    }
  }
  else if (*param_2 == 0x14) {
    *in_D1 = 0x55555555;
  }
  return;
}



void FUN_00007ece(void)

{
  undefined4 *in_D0;
  int iVar1;
  bool bVar2;
  undefined4 local_14;
  
  FUN_0000837a();
  iVar1 = 0xe46ccb;
  do {
    bVar2 = iVar1 != 0;
    iVar1 = iVar1 + -1;
  } while (bVar2);
  FUN_00008392();
  if (local_14 == 0x5a) {
    *in_D0 = 0;
  }
  else {
    *in_D0 = 0x15;
  }
  return;
}



void FUN_00007f2c(void)

{
  undefined4 *in_D0;
  char local_15;
  ushort local_14;
  ushort local_12;
  undefined4 *local_10;
  
  *in_D0 = 0;
  local_15 = '\0';
  local_12 = 2;
  local_10 = in_D0;
  while ((local_12 < 0x15 && (local_15 == '\0'))) {
    FUN_00008246();
    local_12 = local_12 + 1;
  }
  if (local_15 == '\0') {
    local_12 = 2;
    while ((local_12 < 0x15 && (local_14 = local_12, local_15 == '\0'))) {
      while ((local_14 = local_14 + 1, local_14 < 0x15 && (local_15 == '\0'))) {
        FUN_000082d4(&local_15);
      }
      local_12 = local_12 + 1;
    }
  }
  if (local_15 != '\0') {
    switch(local_12) {
    case 3:
      *local_10 = 1;
      break;
    case 4:
      *local_10 = 2;
      break;
    case 5:
      *local_10 = 3;
      break;
    case 6:
      *local_10 = 4;
      break;
    case 7:
      *local_10 = 5;
      break;
    case 8:
      *local_10 = 6;
      break;
    case 9:
      *local_10 = 7;
      break;
    case 10:
      *local_10 = 8;
      break;
    case 0xb:
      *local_10 = 9;
      break;
    case 0xc:
      *local_10 = 10;
      break;
    case 0xd:
      *local_10 = 0xb;
      break;
    case 0xe:
      *local_10 = 0xc;
      break;
    case 0xf:
      *local_10 = 0xd;
      break;
    case 0x10:
      *local_10 = 0xe;
      break;
    case 0x11:
      *local_10 = 0xf;
      break;
    case 0x12:
      *local_10 = 0x10;
      break;
    case 0x13:
      *local_10 = 0x11;
      break;
    case 0x14:
      *local_10 = 0x12;
      break;
    case 0x15:
      *local_10 = 0x13;
    }
  }
  return;
}



void FUN_00008184(undefined4 param_1)

{
  undefined4 *in_D0;
  undefined4 *in_D1;
  undefined4 *local_14;
  
  if (((undefined4 *)0x7fffff < in_D0) && (local_14 = in_D0, in_D1 < (undefined4 *)0xa00000)) {
    for (; local_14 + 1 < in_D1; local_14 = local_14 + 1) {
      *local_14 = param_1;
    }
  }
  return;
}



void FUN_000081d2(int param_1,undefined4 *param_2,int *param_3,int *param_4)

{
  int iVar1;
  int *in_D0;
  int *in_D1;
  
  *param_4 = 0;
  if (((int *)0x7fffff < in_D0) && (in_D1 < (int *)0xa00000)) {
    while ((in_D0 + 1 < in_D1 && (*param_4 == 0))) {
      iVar1 = *in_D0;
      if (iVar1 != param_1) {
        *param_4 = 0x14;
        *param_3 = iVar1;
        *param_2 = in_D0;
      }
      in_D0 = in_D0 + 1;
    }
  }
  return;
}



void FUN_00008246(void)

{
  undefined1 *in_D1;
  undefined4 local_20;
  undefined4 local_1c;
  
  *in_D1 = 0;
  FUN_0000837a();
  FUN_0000837a();
  FUN_00008392();
  FUN_00008392();
  if ((local_1c != 0x5a5a5a5a) || (local_20 != -0x5a5a5a5b)) {
    *in_D1 = 1;
  }
  return;
}



void FUN_000082d4(undefined1 *param_1)

{
  undefined4 local_24;
  undefined4 local_20;
  
  *param_1 = 0;
  FUN_0000837a();
  FUN_0000837a();
  FUN_0000837a();
  FUN_00008392();
  FUN_00008392();
  if ((local_20 != 0) || (local_24 != 0x11111111)) {
    *param_1 = 1;
  }
  return;
}



void FUN_0000837a(void)

{
  undefined4 *in_D0;
  undefined4 in_D1;
  
  *in_D0 = in_D1;
  return;
}



void FUN_00008392(void)

{
  undefined4 *in_D0;
  undefined4 *in_D1;
  
  *in_D1 = *in_D0;
  return;
}



void FUN_000083ac(void)

{
  undefined4 *in_D0;
  undefined4 local_14;
  undefined4 local_10;
  
  FUN_0000863c();
  FUN_00008696();
  if (local_10 == local_14) {
    *in_D0 = 0;
  }
  else {
    *in_D0 = 1;
  }
  return;
}



void FUN_000083f2(void)

{
  int *in_D0;
  undefined4 local_10;
  
  FUN_000083ac();
  if (*in_D0 != 0) {
    FUN_00008464();
    if (local_10 != 0) {
      *in_D0 = local_10;
    }
  }
  return;
}



void FUN_00008464(void)

{
  undefined4 *in_D0;
  char local_15;
  ushort local_14;
  ushort local_12;
  undefined4 *local_10;
  
  *in_D0 = 0;
  local_15 = '\0';
  local_12 = 0;
  local_10 = in_D0;
  while ((local_12 < 0x10 && (local_15 == '\0'))) {
    FUN_000086b4();
    local_12 = local_12 + 1;
  }
  if (local_15 == '\0') {
    local_12 = 0;
    while ((local_12 < 0x10 && (local_14 = local_12, local_15 == '\0'))) {
      while ((local_14 = local_14 + 1, local_14 < 0x10 && (local_15 == '\0'))) {
        FUN_00008744(&local_15);
      }
      local_12 = local_12 + 1;
    }
  }
  if (local_15 != '\0') {
    switch(local_12) {
    case 1:
      *local_10 = 2;
      break;
    case 2:
      *local_10 = 3;
      break;
    case 3:
      *local_10 = 4;
      break;
    case 4:
      *local_10 = 5;
      break;
    case 5:
      *local_10 = 6;
      break;
    case 6:
      *local_10 = 7;
      break;
    case 7:
      *local_10 = 8;
      break;
    case 8:
      *local_10 = 9;
      break;
    case 9:
      *local_10 = 10;
      break;
    case 10:
      *local_10 = 0xb;
      break;
    case 0xb:
      *local_10 = 0xc;
      break;
    case 0xc:
      *local_10 = 0xd;
      break;
    case 0xd:
      *local_10 = 0xe;
      break;
    case 0xe:
      *local_10 = 0xf;
      break;
    case 0xf:
      *local_10 = 0x10;
      break;
    case 0x10:
      *local_10 = 0x11;
    }
  }
  return;
}



void FUN_0000863c(void)

{
  int *in_D0;
  undefined1 local_19;
  undefined4 local_18;
  undefined4 local_14;
  
  local_14 = 0;
  for (local_18 = 0; local_18 < 0xfffc; local_18 = local_18 + 1) {
    FUN_00008836();
    local_14 = local_14 + (uint)local_19;
  }
  *in_D0 = local_14;
  return;
}



void FUN_00008696(void)

{
  FUN_00008850();
  return;
}



void FUN_000086b4(void)

{
  char *in_D1;
  undefined1 local_1e;
  undefined1 local_1d;
  undefined4 local_14;
  
  *in_D1 = '\x01';
  local_14 = 0;
  while ((local_14 < 0x10000 && (*in_D1 != '\0'))) {
    FUN_00008836();
    FUN_00008836();
    if (local_1e != local_1d) {
      *in_D1 = '\0';
    }
    local_14 = local_14 + 1;
  }
  return;
}



void FUN_00008744(char *param_1)

{
  undefined1 local_23;
  undefined1 local_22;
  undefined1 local_21;
  undefined4 local_14;
  
  *param_1 = '\x01';
  local_14 = 0;
  while ((local_14 < 0x10000 && (*param_1 != '\0'))) {
    FUN_00008836();
    FUN_00008836();
    FUN_00008836();
    if ((local_21 != local_22) && (local_21 != local_23)) {
      *param_1 = '\0';
    }
    local_14 = local_14 + 1;
  }
  return;
}



void FUN_00008836(void)

{
  undefined1 *in_D0;
  undefined1 *in_D1;
  
  *in_D1 = *in_D0;
  return;
}



void FUN_00008850(void)

{
  undefined4 *in_D0;
  undefined4 *in_D1;
  
  *in_D1 = *in_D0;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0000886a(char *param_1)

{
  undefined4 in_D0;
  undefined4 in_D1;
  int local_20;
  char *local_1c;
  
  for (local_20 = 0; local_20 < 0xf0; local_20 = local_20 + 1) {
  }
  local_1c = &DAT_009f1016;
  _DAT_009f100e = in_D0;
  _DAT_009f1012 = in_D1;
  for (; *param_1 != '\0'; param_1 = param_1 + 1) {
    *local_1c = *param_1;
    local_1c = local_1c + 1;
  }
  return;
}



void FUN_000088ec(void)

{
  return;
}


