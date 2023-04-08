public class SystemInformation
{
    private System.Diagnostics.PerformanceCounter m_memoryCounter;
    private System.Diagnostics.PerformanceCounter m_CPUCounter;

    public SystemInformation()
    {
        m_memoryCounter = new System.Diagnostics.PerformanceCounter();
        m_memoryCounter.CategoryName = "Memory";
        m_memoryCounter.CounterName = "Available MBytes";

        m_CPUCounter = new System.Diagnostics.PerformanceCounter();
        m_CPUCounter.CategoryName = "Processor";
        m_CPUCounter.CounterName = "% Processor Time";
        m_CPUCounter.InstanceName = "_Total";
    }

    public float GetAvailableMemory()
    {
        return m_memoryCounter.NextValue();
    }

    public float GetCPULoad()
    {
        return m_CPUCounter.NextValue();
    }

    public float GetCPUTemperature()
    {
        //...
        return 0;
    }
}