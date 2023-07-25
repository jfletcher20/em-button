using System.IO.Ports;
using System.Linq;
using System.Management;
using TMPro;
using RJCP.IO.Ports;
using UnityEngine;

public class SerialMonitor : MonoBehaviour {

    string deviceName = "USB-SERIAL CH340";
    private int baudRate = 115200;

    SerialPort serial;
    public GameObject text;


    PortDescription search() => SerialPortStream.GetPortDescriptions().FirstOrDefault(p => p.Description.Contains(deviceName));

    void Start() {
        serial = new SerialPort(search().Port, baudRate);
    }

    void Update() {
        text.GetComponent<TextMeshProUGUI>().text = search().ToString();
    }
}
