using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ThreadAbortTest
{
    class MyApplication
    {
        public enum ErrorModes
        {
            SEM_FAILCRITICALERRORS = NativeMethods.SEM_FAILCRITICALERRORS,
            SEM_NOGPFAULTERRORBOX = NativeMethods.SEM_NOGPFAULTERRORBOX,
            SEM_NOALIGNMENTFAULTEXCEPT = NativeMethods.SEM_NOALIGNMENTFAULTEXCEPT,
            SEM_NOOPENFILEERRORBOX = NativeMethods.SEM_NOOPENFILEERRORBOX
        }

        public static void SetDefaultErrorMode()
        {
            var errorMode = GetErrorMode();
            SetErrorMode(errorMode | ErrorModes.SEM_FAILCRITICALERRORS | ErrorModes.SEM_NOGPFAULTERRORBOX);
        }

        public static ErrorModes SetErrorMode(ErrorModes errorMode)
        {
            return (ErrorModes)NativeMethods.SetErrorMode((int)errorMode);
        }

        public static ErrorModes GetErrorMode()
        {
            try
            {
                return (ErrorModes)NativeMethods.GetErrorMode();
            }
            catch (EntryPointNotFoundException)
            {
            }

            int result = NativeMethods.SetErrorMode(0);
            NativeMethods.SetErrorMode(result);
            return (ErrorModes)result;
        }
    }
}
