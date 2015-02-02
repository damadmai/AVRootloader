unit AVRootIntf;

interface

uses Windows, Classes;

const
// IApplication.Output() Code
  ocInfo              = $10000000;
  ocError             = $11000000;
  ocOthers            = $12000000;
// RetCode Flags
  msCrypt             = $01;
  msCryptFlash        = $02;
  msCryptEeprom       = $04;
  msVersioning        = $08;
// Return Codes of AVR Bootloader Software
  SUCCESS             = $30;
  ERRORVERIFY         = $C0;
  ERRORCOMMAND        = $C1;
  ERRORCRC            = $C2;
  ERRORBOUNDS         = $C3;
  ERRORCRYPT          = $C4;
  ERRORPROG           = $C5;
  ERRORVERSION        = $C6;
  ERRORUNKNOWN        = $CF;

  RETMASK             = $F0;

// IDevice.Support Flags
  sfCrypt             = $0001;  // Cryptography supported
  sfCryptFlash        = $0002;  // FLASH Write must be encrypted, unencrypted FLASH write not supported
  sfCryptEeprom       = $0004;  // EEPROM Write must be encrypted, unencrypted EEPROM write not supported
  sfVersioning        = $0008;  // Versioning supported
  sfReadEeprom        = $0100;  // command EEPROM Read supported, examined at runtime
  sfWriteEeprom       = $0200;  // command EEPROM Write supported, examined at runtime
  sfReadRam           = $0400;  // command RAM Read supported, examined at runtime
  sfVerifyFlash       = $0800;  // command Verify FLASH supported, examined at runtime

type
  // RS232 encapsulation of windows COM port
  TCRCFlag  = (crcReset, crcSend);
  TCRCFlags = set of TCRCFlag;

  ICOM = interface
    procedure Flush; stdcall;
    procedure Purge; stdcall;

    procedure SetTimeout(Value: Cardinal; const ProcName: WideString = ''); stdcall;
    procedure SetParams(Baudrate: Cardinal; Parity: Byte = NOPARITY; Databits: Byte = 8; Stopbits: Byte = ONESTOPBIT; const ProcName: WideString = ''); stdcall;
    procedure SetEchoMode(Value: Bool); stdcall;
    function  EchoMode: Bool; stdcall;

    procedure SetDTR(Value: Bool); stdcall;
    procedure SetRTS(Value: Bool); stdcall;
    procedure WriteData(Buffer: Pointer; Size: Integer; Flags: TCRCFlags = []; const ProcName: WideString = ''); stdcall;
    procedure WriteByte(Value: Byte; Flags: TCRCFlags = []; const ProcName: WideString = ''); stdcall;
    procedure WriteChar(Value: Char; Flags: TCRCFlags = []; const ProcName: WideString = ''); stdcall;
    procedure WriteWord(Value: Word; Flags: TCRCFlags = []; const ProcName: WideString = ''); stdcall;
    procedure WriteLong(Value: Cardinal; Flags: TCRCFlags = []; const ProcName: WideString = ''); stdcall;
    procedure WriteCRC(const ProcName: WideString = ''); stdcall;
    procedure ResetCRC; stdcall;

    procedure ReadData(Buffer: Pointer; Size: Integer; Flags: TCRCFlags = []; const ProcName: WideString = ''); stdcall;
    function  ReadByte(Flags: TCRCFlags = []; const ProcName: WideString = ''): Byte; stdcall;
    function  ReadChar(Flags: TCRCFlags = []; const ProcName: WideString = ''): Char; stdcall;
    function  ReadWord(Flags: TCRCFlags = []; const ProcName: WideString = ''): Word; stdcall;
    function  ReadLong(Flags: TCRCFlags = []; const ProcName: WideString = ''): Cardinal; stdcall;
    function  ReadCRC(const ProcName: WideString = ''): Bool; stdcall;
  end;

  // Timeout Record for IApplication
  TTimeouts = packed record
    Baudrate: Integer;
    Connect: Integer;
    Base: Integer;
    Erase: Integer;
    Flash: Integer;
    Eeprom: Integer;
    Buffer: Integer;
    AppCmd: Integer;
    KeepAlive: Integer;
    RTSPulse: Integer;
    RTSInterval: Integer;
    ConnectTrials: Integer;
    MaxPacketSize: Integer;
    Options: Integer;
  end;

  // Application Callback Interface, must be provided to OpenAVRootloader()
  IApplication = interface
    ['{62DEB67D-8AB2-476E-9CB6-F582A508B1F7}']
    function ProcessMessages: Bool; stdcall;
    procedure Changed; stdcall;
    procedure Output(const Msg: WideString; Code: Integer); stdcall;

    function GetFLASHFileName: WideString; stdcall;
    function GetEEPROMFileName: WideString; stdcall;
    function GetACYFileName: WideString; stdcall;
    function GetPassword: WideString; stdcall;
    function GetBootSign: WideString; stdcall;
    function GetTimeouts: TTimeouts; stdcall;
    function GetAppCmd: WideString; stdcall;
    function GetAppCmdResponse: WideString; stdcall;
    function GetAppVersion(Masked: Bool = False): Integer; stdcall;
    function GetACYInfo: WideString; stdcall;

    function OpenCommunication(Index: Integer): ICOM; stdcall;
  end;

  // individual Commands to send to AVR Bootloader software, part of IAVRootloader
  ICommandSet = interface
    ['{E9C64A3F-306C-4694-B250-FC57D2CB2DEB}']
    function SetAddr(Address: Integer): Bool; stdcall;
    function SetBuffer(Buffer: Pointer; Size: Integer; Code: Byte = 0): Bool; stdcall;
    function Run(Code: Byte = 1): Bool; stdcall;
    function EraseFlash(Pages: Byte = 1): Bool; stdcall;
    function VerifyFlash(Pages: Byte = 1): Bool; stdcall;
    function WriteFlash(Pages: Byte = 1): Bool; stdcall;
    function WriteEeprom(Size: Integer; Pages: Byte = 1): Bool; stdcall;
    function ReadEeprom(Buffer: Pointer; Size: Integer; Address: Integer = 0): Bool; stdcall;
    function ReadRam(Buffer: Pointer; Size: Integer; Address: Integer = 0): Bool; stdcall;
  end;

  // connected Device Information, part of IAVRootloader
  IDevice = interface
    ['{9EC8A92B-F6BB-47F3-A9C9-DF8F4F481F49}']
    function Signature: Integer; stdcall;
    function Name: WideString; stdcall;
    function Info: WideString; stdcall;
    function FlashSize: Integer; stdcall;
    function AppFlashSize: Integer; stdcall;
    function AppVersion: Integer; stdcall;
    function AppVersionString: WideString; stdcall;
    function EepromSize: Integer; stdcall;
    function RamSize: Integer; stdcall;
    function RamStartAddress: Integer; stdcall;
    function PageSize: Integer; stdcall;
    function BufferSize: Integer; stdcall;
    function Version: Integer; stdcall;
    function UseBootSection: Bool; stdcall;
    function RetCode: Byte; stdcall;
    function Support: Integer; stdcall;
    function XMLFileName: WideString; stdcall;
  end;

  TMode = (moDisconnected, moConnecting, moConnected, moWorking, moTimer, moAbort);

  // Bootloader Interface
  IAVRootloader = interface            
    ['{3A2E99C2-CE9E-407B-8943-A6D5EB1F6B7A}']
    function  Mode: TMode; stdcall;

    function  DoConnect(Working: Bool = False): Bool; stdcall;
    procedure DoDisconnect; stdcall;
    procedure DoAbort; stdcall;

    function  DoProgram(EraseFlash: Bool; VerifyFlash: Bool): Bool; stdcall;
    function  DoCompile(EraseFlash: Bool; VerifyFlash: Bool): Bool; stdcall;
    function  DoVerifyFlash: Bool; stdcall;
    function  DoEraseFlash: Bool; stdcall;
    function  DoEraseEeprom: Bool; stdcall;

    function  COM: ICOM; stdcall;
    function  Device: IDevice; stdcall;
    function  Command: ICommandSet; stdcall;
  end;


function OpenCOM(const Port: WideString; const Application: IApplication): ICOM; stdcall; external 'AVRootloader.dll';
function OpenAVRootloader(const Application: IApplication): IAVRootloader; stdcall; external 'AVRootloader.dll';

implementation

end.
