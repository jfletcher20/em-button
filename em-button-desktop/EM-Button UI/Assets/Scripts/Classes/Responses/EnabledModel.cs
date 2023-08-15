using System.Collections.Generic;

/// <summary>
/// Response when it pertains to device status (all OLED Monitor data and more).
/// </summary>
[System.Serializable]
public class EnabledResponseModel : STPResponseModel {
    public new EnabledDataModel data;
    public override Dictionary<string, object> toDictionary() {
        Dictionary<string, object> result = new Dictionary<string, object>();
        result.Add("status", status);
        result.Add("data", data.toDictionary());
        return result;
    }
}

[System.Serializable]
public class EnabledDataModel : STPDataModel {
    public bool enabled;
    public override Dictionary<string, object> toDictionary() {
        Dictionary<string, object> result = new Dictionary<string, object>();
        result.Add("message", message);
        result.Add("enabled", enabled);
        return result;
    }
    public enum Keys { enabled }
}