using System;

namespace MemoryStream1
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                MemoryStream1.Run(args);
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex.ToString());
            }
        }
    }
}
