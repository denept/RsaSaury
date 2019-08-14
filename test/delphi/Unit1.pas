unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type
  TForm1 = class(TForm)
    Memo1: TMemo;
    Button1: TButton;
    Memo2: TMemo;
    Label1: TLabel;
    Label2: TLabel;
    Memo3: TMemo;
    Label3: TLabel;
    Button2: TButton;
    Memo4: TMemo;
    Label4: TLabel;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }

  end;

var
  Form1: TForm1;

  function RsaEncrypt(const text:PAnsiChar;const textout:PAnsiChar):Integer;stdcall; external 'rsaSaury.dll'; //
  function RsaDecrypt(const text:PAnsiChar;const textout:PAnsiChar):Integer;stdcall; external 'rsaSaury.dll';
  function ToUTF8Encode(str: string): string;

implementation

{$R *.dfm}

function ToUTF8Encode(str: string): string;
var
  b: Byte;
begin
  for b in BytesOf(UTF8Encode(str)) do
    Result := Format('%s%%%.2x', [Result, b]);
end;

procedure TForm1.Button1Click(Sender: TObject);
var
text: PAnsiChar;
textout: PAnsiChar;
ret: Integer;
strUTF8: AnsiString;
begin

//Memo3.text := Memo1.text;
//text:=PAnsiChar(Memo1.text);
strUTF8 := UTF8Encode(Memo1.text);
text:= PAnsiChar(AnsiString(strUTF8));



//Memo3.text := text;
GetMem(textout,100000);//分配内存
//textout:='0000000000';
ret := RsaEncrypt(text,@textout);
Memo3.text := AnsiString(textout);
FreeMem(textout); //释放;
//Memo3.text := AnsiString(RsaEncrypt(text));
end;

procedure TForm1.Button2Click(Sender: TObject);
var
text: PAnsiChar;
textout: PAnsiChar;
ret: Integer;
begin

text:= PAnsiChar(AnsiString(Memo2.text));
GetMem(textout,100000);//分配内存
ret := RsaDecrypt(text,@textout);
Memo4.text := UTF8Decode(AnsiString(textout));
FreeMem(textout); //释放;
end;

end.
