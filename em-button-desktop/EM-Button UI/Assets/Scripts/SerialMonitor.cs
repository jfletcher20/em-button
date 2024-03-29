using System.IO.Ports;
using System.Linq;
using System.Management;
using TMPro;
using RJCP.IO.Ports;
using UnityEngine;
using System.Collections.Generic;

public class SerialMonitor : MonoBehaviour {

    string deviceName = "USB-SERIAL CH340";
    private int baudRate = 115200;

    SerialPort port;
    public TextMeshProUGUI monitorDataComponent;
    public TextMeshProUGUI portDataComponent;
    private Color initialPortDataComponentColor;
    
    private void OnEnable() {
        initialPortDataComponentColor = portDataComponent.color;
        initSerialPort();
    }

    private const float timerDuration = 0.1f;
    private float timer = timerDuration;

    void Update() {
        timer -= Time.deltaTime;
        serialDataManagement();
    }

    private void serialDataManagement() {
        if (timer <= 0) {
            _port();
            if (port != null) _log();
            timer = timerDuration;
        }
    }

    PortDescription findSerialPort() => SerialPortStream.GetPortDescriptions().FirstOrDefault(p => p.Description.Contains(deviceName));

    private string _portNotFound = "Port not found. Please plug in the device.";
    public void openPort(PortDescription discoveredPort) {
        port = new SerialPort(discoveredPort.Port, baudRate);
        port.ReadTimeout = 1;
        attemptToOpenPort();
    }

    void initSerialPort() {
        PortDescription discoveredPort = findSerialPort();
        if (discoveredPort == null) {
            portNotFound();
            return;
        }
        portDataComponent.text = portName(discoveredPort.ToString());
        currentMonitorData["port"] = portDataComponent.text;
        openPort(discoveredPort);
    }

    public void portNotFound() {
        portDataComponent.text = _portNotFound;
        currentMonitorData["port"] = portDataComponent.text;
    }

    void initSerialPort(PortDescription discoveredPort) {
        if (discoveredPort == null) {
            portNotFound();
            return;
        }
        portDataComponent.text = portName(discoveredPort.ToString());
        currentMonitorData["port"] = portDataComponent.text;
        openPort(discoveredPort);
    }

    public string portName(string discoveredPort) => discoveredPort.Substring(0, discoveredPort.IndexOf(")") + 1);

    private void attemptToOpenPort() {
        try {
            portDataComponent.color = initialPortDataComponentColor;
            port.Open();
        } catch {
            portDataComponent.text = "ERROR: Port is in use by another process.";
            monitorDataComponent.text = "ERROR\n\nThe port is currently in use by another process.\n"
                + "Close any other programs and click the settings button to refresh.";
            portDataComponent.color = Color.red;
        }
    }

    private Dictionary<float, string> logs = new Dictionary<float, string>();
    public KeyValuePair<float, string> log { get => logs.Last<KeyValuePair<float, string>>(); }

    private string _latestLogVal = "";

    private PortDescription _portDesc;
    private void _port() {
        if (port == null) {
            initSerialPort();
            return;
        } else {
            _portDesc = findSerialPort();
            if (_portDesc != null) {
                if (portName(_portDesc.ToString()) != currentMonitorData["port"]) {
                    initSerialPort(_portDesc);
                }
            } else portNotFound();
        }
    }

    private void _log() {
        try {
            _latestLogVal = port.ReadLine();
        } catch {
            return;
        }
        if (_latestLogVal.Length > 1 && _latestLogVal.StartsWith("STP1.0")) {
            logs.Add(Time.time, _latestLogVal);
            print(_latestLogVal);
        }
    }

    public void sendCommand(STPCommand command) {
        try {
            if (!port.IsOpen) {
                port.Open();
        } else
            port.Write(command.ToString());
        } catch {
            Debug.LogWarning("Failed to send command. Is the port initialized and open?");
        }
    }

    public KeyValuePair<float, string> read() => log;
    public string latest {
        get {
            try { return read().Value.Substring(read().Value.IndexOf("{")); } catch { return ""; }
        }
    }
    public float latestUpdate {
        get {
            try { return read().Key; } catch { return 0; }
        }
    }

    public Dictionary<string, string> currentMonitorData = new Dictionary<string, string>();
    public void updateMonitor(Dictionary<string, string> data) {
        bool refresh = false;
        if (data.Keys != currentMonitorData.Keys)
            foreach (string key in data.Keys) {
                if (checkForKey(new KeyValuePair<string, string>(key, data[key]))) {
                    refresh = true;
                }
            }
        if (refresh) refreshMonitor();
    }

    public void updateMonitor(Dictionary<string, object> data) {
        bool refresh = false;
        if (data.Keys.ToList<string>() != currentMonitorData.Keys.ToList<string>())
            foreach (string key in data.Keys) {
                if (checkForKey(new KeyValuePair<string, string>(key, data[key].ToString()))) {
                    refresh = true;
                }
            }
        if (refresh) refreshMonitor();
    }

    public void updateMonitor(KeyValuePair<string, string> dataPoint) {
        if (checkForKey(dataPoint)) refreshMonitor();
    }

    private bool checkForKey(KeyValuePair<string, string> dataPoint) {
        if (!currentMonitorData.Keys.Contains<string>(dataPoint.Key)) {
            currentMonitorData.Add(dataPoint.Key, dataPoint.Value);
            return true;
        } else if (currentMonitorData[dataPoint.Key] != dataPoint.Value) {
            currentMonitorData[dataPoint.Key] = dataPoint.Value;
            return true;
        }
        return false;
    }

    public void refreshMonitor() {
        monitorDataComponent.text = "";
        foreach (string key in currentMonitorData.Keys) {
            if (key == "port") continue;
            monitorDataComponent.text += key.ToUpper() + ": " + currentMonitorData[key] + "\n";
        }
    }

    //public void refreshMonitor(Dictionary<string, object> data) {
    //    monitorDataComponent.text = "";
    //    displayDictionaryData(data, "");
    //}

    //private void displayDictionaryData(Dictionary<string, object> dictionary, string prefix) {
    //    foreach (var kvp in dictionary) {
    //        if (kvp.Value is Dictionary<string, object> nestedDict) {
    //            displayDictionaryData(nestedDict, prefix + kvp.Key + ".");
    //        } else if (kvp.Value is List<object> nestedList) {
    //            for(int i = 0; i < nestedList.Count; i++) {
    //                if(kvp.Value is Dictionary<string, object> yetAnotherNestedDict) {
    //                    displayDictionaryData(yetAnotherNestedDict, prefix + kvp.Key + ".");
    //                } else {
    //                    monitorDataComponent.text += (prefix + kvp.Key).ToUpper() + ": " + kvp.Value.ToString() + "\n";
    //                    currentMonitorData[kvp.Key] = kvp.Value.ToString();
    //                }
    //            }
    //        } else {
    //            monitorDataComponent.text += (prefix + kvp.Key).ToUpper() + ": " + kvp.Value + "\n";
    //            currentMonitorData[kvp.Key] = kvp.Value.ToString();
    //        }
    //    }
    //}

}

