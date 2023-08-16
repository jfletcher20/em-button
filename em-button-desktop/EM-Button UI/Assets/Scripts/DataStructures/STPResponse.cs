using System;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
///  Base response class
/// </summary>
[System.Serializable]
public class STPResponseModel {
    public int status;
    public STPDataModel data;
    public virtual Dictionary<string, object> toDictionary() {
        Dictionary<string, object> result = new Dictionary<string, object>();
        result.Add("status", status);
        result.Add("data", data.toDictionary());
        return result;
    }
}
[System.Serializable]
public class STPDataModel {
    public string message;
    public virtual Dictionary<string, object> toDictionary() {
        Dictionary<string, object> result = new Dictionary<string, object>();
        result.Add("message", message);
        return result;
    }
}