using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;

namespace testrsa
{
    public class DllInvoke
    {
        [DllImport("kernel32.dll")]
        private extern static IntPtr LoadLibrary(String path);
        [DllImport("kernel32.dll")]
        private extern static IntPtr GetProcAddress(IntPtr lib, String funcName);
        [DllImport("kernel32.dll")]
        private extern static bool FreeLibrary(IntPtr lib);
        private IntPtr hLib;

        public DllInvoke(String DLLPath)
        {
            hLib = LoadLibrary(DLLPath);
        }
        ~DllInvoke()
        {
            FreeLibrary(hLib);
        }
        //将要执行的函数转换为委托
        public Delegate Invoke(String APIName, Type t)
        {
            IntPtr api = GetProcAddress(hLib, APIName);
            return (Delegate)Marshal.GetDelegateForFunctionPointer(api, t);
        }
    }
    class Program
    {
        //定义委托变量
        //public delegate int C();
        [DllImport("./rsaSaury.dll")]
        public static extern int RsaEncrypt(byte[] str, ref string outstr);
        [DllImport("./rsaSaury.dll")]
        public static extern int RsaDecrypt(byte[] str, byte[] outstr);
        [DllImport("./rsaSaury.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern int Test(StringBuilder str);

        static void Main(string[] args)
{
            //DllInvoke dllInvoke = new DllInvoke("./rsaSaury.dll"); //也可以把Dll保存在bin下，用相对路径转换为绝对路径
            char[] chars = new char[8000];
            string outstr = new String(chars);

            int result = RsaEncrypt(Encoding.UTF8.GetBytes("文aas中文dwwewed\0"), ref outstr);
            Console.WriteLine(result);
            Console.WriteLine(outstr);
            Byte[] str = Encoding.UTF8.GetBytes(outstr+"\0");
            byte[] outstr2 = new byte[8000];
            int result2 = RsaDecrypt(str, outstr2);
            String decodedString2 = Encoding.UTF8.GetString(outstr2);
            decodedString2 = Regex.Replace(decodedString2, "\0", String.Empty);
            
            Console.WriteLine(result2);
            Console.WriteLine(decodedString2);
        }
    }
}
