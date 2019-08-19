using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace FileReadWriteTest
{
    abstract class MyHashAlgorithm : IDisposable
    {
        public string Name { get; protected set; }

        protected abstract HashAlgorithm Create();

        private HashAlgorithm HashAlgorithm;
        private byte[] Hash;

        public void Initialize()
        {
            if (HashAlgorithm != null && HashAlgorithm.CanReuseTransform)
                HashAlgorithm.Initialize();
            else
                HashAlgorithm = Create();
        }

        public void TransformBlock(byte[] buffer, int offset, int count)
        {
            if (!HashAlgorithm.CanTransformMultipleBlocks)
                throw new NotSupportedException(string.Format("{0}: Cannot transform multiple blocks.", Name));
            HashAlgorithm.TransformBlock(buffer, offset, count, buffer, offset);
        }

        public void TransformFinalBlock(byte[] buffer, int offset, int count)
        {
            HashAlgorithm.TransformFinalBlock(buffer, offset, count);
            Hash = HashAlgorithm.Hash;
        }

        public string FormatHash()
        {
            var sb = new StringBuilder(Hash.Length * 2);
            for (int i = 0; i < Hash.Length; ++i)
                sb.Append(Hash[i].ToString("X2"));
            return sb.ToString();
        }

        #region IDisposable Support

        private bool _disposed = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (disposing)
                {
                    if (HashAlgorithm != null)
                    {
                        HashAlgorithm.Dispose();
                        HashAlgorithm = null;
                    }
                }

                _disposed = true;
            }
        }

        public void Dispose()
        {
            Dispose(true);
        }

        #endregion
    }

    class MyMD5 : MyHashAlgorithm
    {
        public static new string Name { get { return "MD5"; } }

        public MyMD5()
        {
            base.Name = Name;
        }

        protected override HashAlgorithm Create()
        {
            return MD5.Create();
        }
    }

    class MySHA1 : MyHashAlgorithm
    {
        public static new string Name { get { return "SHA1"; } }

        public MySHA1()
        {
            base.Name = Name;
        }

        protected override HashAlgorithm Create()
        {
            return SHA1.Create();
        }
    }

    class MySHA256 : MyHashAlgorithm
    {
        public static new string Name { get { return "SHA256"; } }

        public MySHA256()
        {
            base.Name = Name;
        }

        protected override HashAlgorithm Create()
        {
            return SHA256.Create();
        }
    }

    class MySHA384 : MyHashAlgorithm
    {
        public static new string Name { get { return "SHA384"; } }

        public MySHA384()
        {
            base.Name = Name;
        }

        protected override HashAlgorithm Create()
        {
            return SHA384.Create();
        }
    }

    class MySHA512 : MyHashAlgorithm
    {
        public static new string Name { get { return "SHA512"; } }

        public MySHA512()
        {
            base.Name = Name;
        }

        protected override HashAlgorithm Create()
        {
            return SHA512.Create();
        }
    }

    class MySimpleHash : MyHashAlgorithm
    {
        public static new string Name { get { return "MYSIMPLEHASH"; } }

        public MySimpleHash()
        {
            base.Name = Name;
        }

        protected override HashAlgorithm Create()
        {
            return MySimpleHashAlgorithm.Create();
        }
    }

    class MySimpleHashAlgorithm : HashAlgorithm
    {
        private int hash;

        new static public MySimpleHashAlgorithm Create()
        {
            return Create("FileReadWriteTest.MySimpleHashAlgorithm");
        }

        new static public MySimpleHashAlgorithm Create(string hashName)
        {
            return new MySimpleHashAlgorithm();
        }

        public MySimpleHashAlgorithm()
        {
            HashSizeValue = 32;
        }

        public override void Initialize()
        {
            hash = 0;
        }

        protected override void HashCore(byte[] array, int ibStart, int cbSize)
        {
            for (int i = ibStart, n = i + cbSize; i < n; ++i)
            {
                hash = (hash << 1) ^ array[i];
            }
        }

        protected override byte[] HashFinal()
        {
            var result = BitConverter.GetBytes(hash);
            if (BitConverter.IsLittleEndian)
                Array.Reverse(result);
            return result;
        }
    }
}
