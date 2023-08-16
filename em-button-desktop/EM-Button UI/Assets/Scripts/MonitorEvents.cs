using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MonitorEvents : MonoBehaviour {
    public SerialMonitor serialMonitor;
    private float lastUpdate = 0;
    void Update() {
        if (serialMonitor.latest != "" && serialMonitor.read().Key != lastUpdate) {
            lastUpdate = serialMonitor.latestUpdate;
            //serialMonitor.refreshMonitor(specifyData());
            serialMonitor.updateMonitor(specifyData());
        }
    }

    Dictionary<string, object> empty = new Dictionary<string, object>();
    private Dictionary<string, object> specifyData() {
        string latest = serialMonitor.latest;
        if (latest.Contains("current_state")) {
            DeviceStatusResponseModel status = statusFromJson(latest);
            Dictionary<string, object> dataToDisplay = new Dictionary<string, object>();
            dataToDisplay.Add("Device is on", status.data.current_state.device_enabled);
            dataToDisplay.Add("Bluetooth connected", status.data.current_state.ble_connected);
            dataToDisplay.Add("Current", status.data.current_state.current_value + " (" + status.data.current_state.value_normalized + ")");
            KeyValuePair<string, object> kvp = pressedData(status.data.current_state.button_pressed, status.data.current_state.times_pressed);
            dataToDisplay.Add(kvp.Key, kvp.Value);
            return dataToDisplay;
        } else if (latest.Contains("time_when_pressed")) {
            KeyPressResponseModel keyPressed = keyPressFromJson(latest);
            Dictionary<string, object> dataToDisplay = new Dictionary<string, object>();
            KeyValuePair<string, object> kvp = pressedData(keyPressed.data.actionId, keyPressed.data.times_pressed);
            dataToDisplay.Add(kvp.Key, kvp.Value);
            return dataToDisplay;
        } else if (latest.Contains("ble_connected") && latest.Contains("\"status\":300")) {
            BleConnectionDataModel bleConnected = bleConnectionFromJson(latest);
            Dictionary<string, object> dataToDisplay = new Dictionary<string, object>();
            dataToDisplay.Add("Bluetooth connected", bleConnected.ble_connected);
            return dataToDisplay;
        } else if (latest.Contains("enabled") && latest.Contains("\"status\":204")) {
            EnabledResponseModel deviceEnabled = enabledFromJson(latest);
            Dictionary<string, object> dataToDisplay = new Dictionary<string, object>();
            dataToDisplay.Add("Device is on", deviceEnabled.data.enabled);
            return dataToDisplay;
        } /*else if (latest.Contains("routes")) {
            RoutesResponseModel deviceEnabled = routesFromJson(latest);
            gameObject.GetComponent<SettingsTab>().initRoutes();
            return empty;
        }*/ else {
            return empty;
        }
    }

    KeyValuePair<string, object> pressedData(int button_pressed, int times_pressed) {
        int val = -1;
        try { val = button_pressed; } catch { }
        object value = (val != -1 ? val : "-") + " (x" + times_pressed + ")";
        return new KeyValuePair<string, object>("Pressed", value);
    }

    public Dictionary<string, object> getDictionaryFromJson(string json) {
        if (json.Contains("current_state")) {
            return statusFromJson(json).toDictionary();
        } else if (json.Contains("time_when_pressed")) {
            return keyPressFromJson(json).toDictionary();
        } else if (json.Contains("ble_connected") && json.Contains("\"status\":300")) {
            return bleConnectionFromJson(json).toDictionary();
        } else {
            return JsonUtility.FromJson<STPResponseModel>(json).toDictionary();
        }
    }

    private DeviceStatusResponseModel statusFromJson(string json) => JsonUtility.FromJson<DeviceStatusResponseModel>(json);
    private KeyPressResponseModel keyPressFromJson(string json) => JsonUtility.FromJson<KeyPressResponseModel>(json);
    private BleConnectionDataModel bleConnectionFromJson(string json) => JsonUtility.FromJson<BleConnectionDataModel>(json);
    private EnabledResponseModel enabledFromJson(string json) => JsonUtility.FromJson<EnabledResponseModel>(json);
    private RoutesResponseModel routesFromJson(string json) => JsonUtility.FromJson<RoutesResponseModel>(json);
}
