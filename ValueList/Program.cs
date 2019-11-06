using System;

namespace ValueList
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                ValueList.Test();
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex.ToString());
            }
        }
    }
}
