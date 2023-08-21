//using System.Collections.Generic;

///// <summary>
///// Response when it pertains to device status (all OLED Monitor data and more).
///// </summary>
//[System.Serializable]
//public class UpdatedResponseModel : STPResponseModel {
//    public new UpdatedDataModel data;
//    public override Dictionary<string, object> toDictionary() {
//        Dictionary<string, object> result = new Dictionary<string, object>();
//        result.Add("status", status);
//        result.Add("data", data.toDictionary());
//        return result;
//    }
//}

//[System.Serializable]
//public class UpdatedDataModel : STPDataModel {
//    public bool updated;
//    public override Dictionary<string, object> toDictionary() {
//        Dictionary<string, object> result = new Dictionary<string, object>();
//        result.Add("message", message);
//        result.Add("enabled", updated);
//        return result;
//    }
//    public enum Keys { updated }
//}