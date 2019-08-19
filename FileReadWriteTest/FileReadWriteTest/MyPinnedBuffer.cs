using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FileReadWriteTest
{
    class MyPinnedBuffer : IDisposable
    {
        public MyPinnedBuffer(byte[] buffer)
        {
            Handle = GCHandle.Alloc(buffer, GCHandleType.Pinned);
            Ptr = Handle.AddrOfPinnedObject();
        }

        private GCHandle Handle { get; }
        public IntPtr Ptr { get; }

        #region IDisposable Support

        private bool _disposed = false;

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (disposing)
                {
                    Handle.Free();
                }

                _disposed = true;
            }
        }

        void IDisposable.Dispose()
        {
            Dispose(true);
        }

        #endregion
    }
}
