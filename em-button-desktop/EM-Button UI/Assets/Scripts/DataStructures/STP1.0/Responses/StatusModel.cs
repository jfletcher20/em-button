using System.Collections.Generic;

/// <summary>
/// Response when it pertains to device status (all OLED Monitor data and more).
/// </summary>
[System.Serializable]
public class DeviceStatusResponseModel : STPResponseModel {
    public new StatusDataModel data;
    public override Dictionary<string, object> toDictionary() {
        Dictionary<string, object> result = new Dictionary<string, object>();
        result.Add("status", status);
        result.Add("data", data.toDictionary());
        return result;
    }
}

[System.Serializable]
public class StatusDataModel : STPDataModel {
    public StatusData current_value;
    public override Dictionary<string, object> toDictionary() {
        Dictionary<string, object> result = new Dictionary<string, object>();
        result.Add("message", message);
        result.Add("current_value", current_value.toDictionary());
        return result;
    }
}

[System.Serializable]
public class StatusData {
    public int id, electromagnet, hall_sensor;
    public double electromagnet_power;
    public List<EmbAction> actions;
    public int current_state;
    public int value_normalized;
    public int times_pressed;
    public int button_pressed;
    public bool device_enabled, ble_connected;
    public float time_when_retrieved;
    public enum Keys {
        id,
        electromagnet,
        hall_sensor,
        electromagnet_power,
        actions,
        current_value,
        value_normalized,
        times_pressed,
        button_pressed,
        ble_connected,
        time_when_retrieved,
        device_enabled,
    }

    public Dictionary<string, object> toDictionary() {
        Dictionary<string, object> result = new Dictionary<string, object>();
        result.Add("id", id);
        result.Add("electromagnet", electromagnet);
        result.Add("electromagnet_power", electromagnet_power);
        result.Add("hall_sensor", hall_sensor);

        Dictionary<int, Dictionary<string, object>> actionsList = new Dictionary<int, Dictionary<string, object>>();
        foreach (EmbAction action in actions) {
            actionsList.Add(actions.IndexOf(action), action.toDictionary());
        }

        result.Add("actions", actionsList);

        result.Add("current_state", current_state);
        result.Add("value_normalized", value_normalized);
        result.Add("times_pressed", times_pressed);
        result.Add("button_pressed", button_pressed);
        result.Add("device_enabled", device_enabled);
        result.Add("ble_connected", ble_connected);
        result.Add("time_when_retrieved", time_when_retrieved);

        return result;
    }
}