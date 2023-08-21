using System.Collections.Generic;

/// <summary>
/// Response when it pertains to device status (all OLED Monitor data and more).
/// </summary>
[System.Serializable]
public class DatabaseGetResponseModel : STPResponseModel {
    public new List<EmbButton> data;
    public override Dictionary<string, object> toDictionary() {
        Dictionary<string, object> result = new Dictionary<string, object>();
        result.Add("status", status);
        result.Add("data", new List<Dictionary<string, object>>());
        foreach (var button in data) {
            ((List<Dictionary<string, object>>)result["data"]).Add(button.toDictionary());
        }
        return result;
    }
}

[System.Serializable]
public class DatabaseGetDataModel : STPDataModel {
    List<EmbButton> buttons;
    public override Dictionary<string, object> toDictionary() {
        Dictionary<string, object> result = new Dictionary<string, object>();
        foreach (var button in buttons) {
            result.Add(button.id.ToString(), button.toDictionary());
        }
        return result;
    }
}
