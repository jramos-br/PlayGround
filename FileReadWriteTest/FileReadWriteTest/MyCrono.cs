using System;

namespace FileReadWriteTest
{
    // using (new MyCrono(sw => Console.Out.WriteLine("Elapsed: {0}", sw.Elapsed)))
    // {
    //     /* stuff to to measure */
    // }
    //
    // TimeSpan elapsed = MyCrono.Elapsed(() => /* stuff to to measure */);

    class MyCrono : IDisposable
    {
        private Action<System.Diagnostics.Stopwatch> fn;
        private System.Diagnostics.Stopwatch sw;

        public MyCrono(Action<System.Diagnostics.Stopwatch> action)
        {
            fn = action;
            sw = System.Diagnostics.Stopwatch.StartNew();
        }

        public void Dispose()
        {
            sw.Stop();
            fn(sw);
        }

        public static TimeSpan Elapsed(Action action)
        {
            var sw = System.Diagnostics.Stopwatch.StartNew();
            action();
            sw.Stop();
            return sw.Elapsed;
        }
    }
}
