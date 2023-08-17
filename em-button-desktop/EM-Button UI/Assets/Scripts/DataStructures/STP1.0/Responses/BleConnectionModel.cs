using System.Collections.Generic;

/// <summary>
/// Response when it pertains to device status (all OLED Monitor data and more).
/// </summary>
[System.Serializable]
public class STPBleConnectionResponseModel : STPResponseModel {
    public new BleConnectionDataModel data;
    public override Dictionary<string, object> toDictionary() {
        Dictionary<string, object> result = new Dictionary<string, object>();
        result.Add("status", status);
        result.Add("data", data.toDictionary());
        return result;
    }
}

[System.Serializable]
public class BleConnectionDataModel : STPDataModel {
    public bool ble_connected;
    public override Dictionary<string, object> toDictionary() {
        Dictionary<string, object> result = new Dictionary<string, object>();
        result.Add("message", message);
        result.Add("ble_connected", ble_connected);
        return result;
    }
    public enum Keys { ble_connected }
}