using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ThreadAbortTest
{
    class Program
    {
        #region Data

        private const int ERROR_GEN_FAILURE = 31;
        private const int ERROR_INVALID_PARAMETER = 87;
        private const int ERROR_CONTROL_C_EXIT = 572;
        private const int ERROR_UNHANDLED_EXCEPTION = 574;

        private const string ExceptionSeparator = "----";
        private const string PressEnterToExit = "Pressione ENTER para sair. . . ";

        private const string UnhandledExceptionMessage = "* UnhandledException *";
        private const string ThreadAbortExceptionMessage = "* ThreadAbortException *";

        private static volatile Thread _mainThread;

        #endregion

        #region Handlers

        static Program()
        {
            //AppDomain.CurrentDomain.UnhandledException += UnhandledExceptionEventHandler;
            Console.CancelKeyPress += CancelKeyPressEventHandler;
        }

        private static void UnhandledExceptionEventHandler(object sender, UnhandledExceptionEventArgs e)
        {
            try
            {
                try
                {
                    TraceMessage(UnhandledExceptionMessage);

                    if (e.ExceptionObject is Exception)
                        TraceException(e.ExceptionObject as Exception);
                    else
                        TraceMessage(e.ExceptionObject != null ? e.ExceptionObject.ToString() : "ExceptionObject = (Null)");
                }
                catch (Exception ex) when (!IsThreadAbortException(ex))
                {
                    WriteException(ex, GetMyName());
                }
                finally
                {
                    try
                    {
                        Terminate();
                    }
                    catch (Exception ex)
                    {
                        WriteException(ex, GetMyName());
                    }
                }
            }
            catch (ThreadAbortException ex)
            {
                Thread.ResetAbort();
                WriteMessage(ThreadAbortExceptionMessage);
                WriteException(ex, GetMyName());
            }
            finally
            {
                Environment.Exit(ERROR_UNHANDLED_EXCEPTION);
            }
        }

        private static void CancelKeyPressEventHandler(object sender, ConsoleCancelEventArgs e)
        {
            try
            {
                var mainThread = _mainThread;
                if (mainThread != null && mainThread.IsAlive)
                    mainThread.Abort();
            }
            catch (Exception ex)
            {
                WriteException(ex, GetMyName());
            }
            finally
            {
                e.Cancel = true;
            }
        }

        #endregion

        #region Main

        private static void Main(string[] args)
        {
            Initialize();
            Test5();
            return;
            try
            {
                try
                {
                    //Run();
                }
                catch (Exception ex) when (!IsThreadAbortException(ex))
                {
                    WriteException(ex, GetMyName());
                }
            }
            catch (ThreadAbortException ex)
            {
                Thread.ResetAbort();
                WriteMessage(ThreadAbortExceptionMessage);
                WriteException(ex, GetMyName());
            }
        }

        private static void Run()
        {
            try
            {
                try
                {
                    Initialize();

                    try
                    {
                        Test?.Invoke();
                    }
                    catch (Exception ex) when (!IsThreadAbortException(ex))
                    {
                        WriteException(ex, GetMyName());
                    }
                    finally
                    {
                        Terminate();
                    }
                }
                catch (Exception ex) when (!IsThreadAbortException(ex))
                {
                    WriteException(ex, GetMyName());
                }
            }
            catch (ThreadAbortException ex)
            {
                Thread.ResetAbort();
                WriteMessage(ThreadAbortExceptionMessage);
                WriteException(ex, GetMyName());
            }
            finally
            {
                PromptUser();
            }
        }

        private static void Initialize()
        {
            //MyApplication.SetDefaultErrorMode();

            _mainThread = Thread.CurrentThread;
        }

        private static void Terminate()
        {
            _mainThread = null;
        }

        private static void PromptUser()
        {
            Console.Out.Write(PressEnterToExit);
            Console.Out.Flush();
            Console.In.Read();
        }

        private static bool IsThreadAbortException(Exception ex)
        {
            return ex is ThreadAbortException;
        }

        private static void TraceException(Exception ex, string message = null)
        {
            var exceptionText = GetExceptionText(ex, message);
            WriteMessage(exceptionText);
            Trace.WriteLine(exceptionText);
        }

        private static void WriteException(Exception ex, string message = null)
        {
            var exceptionText = GetExceptionText(ex, message);
            WriteMessage(exceptionText);
        }

        private static string GetExceptionText(Exception ex, string message = null)
        {
            var prefix = message != null ? message + ": " : string.Empty;
            var exceptionText = ex != null ? ex.ToString() : "Exception = (Null)";
            return prefix + exceptionText;
        }

        private static void TraceMessage(string message)
        {
            var nonEmptyText = GetNonEmptyText(message);
            Console.Error.WriteLine(nonEmptyText);
            Trace.WriteLine(nonEmptyText);
        }

        private static void WriteMessage(string message)
        {
            var nonEmptyText = GetNonEmptyText(message);
            Console.Error.WriteLine(nonEmptyText);
        }

        private static string GetNonEmptyText(string message)
        {
            if (message == null)
                message = nameof(WriteMessage) + ": message = (Null)";
            else if (message.Length == 0)
                message = nameof(WriteMessage) + ": message = (Empty)";
            else if (message.Trim().Length == 0)
                message = nameof(WriteMessage) + ": message = (WhiteSpace)";

            return message;
        }

        private static void SetExitCode(int exitCode)
        {
            if (Environment.ExitCode == 0)
                Environment.ExitCode = exitCode;
        }

        private static string GetMyName([CallerMemberName] string callerMemberName = "")
        {
            return callerMemberName;
        }

        #endregion

        static Action Test = Test5;

        #region Test1

        private static void Test1()
        {
            WriteMessage(GetMyName() + ": begin");

            try
            {
                WriteMessage(GetMyName() + ": try");

                while (_mainThread != null)
                    ;
            }
            catch (Exception ex)
            {
                WriteMessage(GetMyName() + ": catch " + ex.GetType().FullName + ": " + ex.Message);
            }
            finally
            {
                WriteMessage(GetMyName() + ": finally");
            }

            WriteMessage(GetMyName() + ": end");
        }

        #endregion

        #region Test2

        private static void Test2()
        {
            WriteMessage(GetMyName() + ": begin");

            try
            {
                WriteMessage(GetMyName() + ": try");

                Thread.Sleep(5000);
            }
            catch (Exception ex)
            {
                WriteMessage(GetMyName() + ": catch " + ex.GetType().FullName + ": " + ex.Message);
            }
            finally
            {
                WriteMessage(GetMyName() + ": finally");
            }

            WriteMessage(GetMyName() + ": end");
        }

        #endregion

        #region Test3

        private static void Test3()
        {
            WriteMessage(GetMyName() + ": begin");

            try
            {
                WriteMessage(GetMyName() + ": try");

                Thread.Sleep(5000);
            }
            catch (Exception ex)
            {
                WriteMessage(GetMyName() + ": catch " + ex.GetType().FullName + ": " + ex.Message);
                WriteMessage(GetMyName() + ": throw Exception in catch");
                throw new Exception("Exception in catch (" + GetMyName() + ")");
            }
            finally
            {
                WriteMessage(GetMyName() + ": finally");
            }

            WriteMessage(GetMyName() + ": end");
        }

        #endregion

        #region Test4

        private static void Test4()
        {
            WriteMessage(GetMyName() + ": begin");

            try
            {
                WriteMessage(GetMyName() + ": try");

                Thread.Sleep(5000);
            }
            catch (Exception ex)
            {
                WriteMessage(GetMyName() + ": catch " + ex.GetType().FullName + ": " + ex.Message);
            }
            finally
            {
                WriteMessage(GetMyName() + ": finally");
                WriteMessage(GetMyName() + ": throw Exception in finally");
                throw new Exception("Exception in finally (" + GetMyName() + ")");
            }

            // WriteMessage(MyName() + ": end");
        }

        #endregion

        #region Test5

        private static void Test5()
        {
            WriteMessage(GetMyName() + ": begin");

            try
            {
                WriteMessage(GetMyName() + ": try");
                Thread.Sleep(5000);
            }
            catch (Exception ex)
            {
                WriteMessage(GetMyName() + ": catch " + ex.GetType().FullName + ": " + ex.Message);
                WriteMessage(GetMyName() + ": throw Exception in catch");
                throw new Exception("Exception in catch (" + GetMyName() + ")");
            }
            finally
            {
                WriteMessage(GetMyName() + ": finally");
                WriteMessage(GetMyName() + ": throw Exception in finally");
                //throw new Exception("Exception in finally (" + GetMyName() + ")");
            }

            // WriteMessage(MyName() + ": end");
        }

        #endregion

        // TODO ACRESCENTAR TESTE DE LEITURA DE CONSOLE. PRECISA DE UM SLEEP PARA QUE A THREAD DO COTROL/C RODE
    }
}
