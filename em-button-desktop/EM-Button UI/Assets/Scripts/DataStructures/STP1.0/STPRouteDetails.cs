using System.Collections.Generic;
using System.Linq;
using UnityEngine;


public class RouteManagement {
    public struct STPRouteDetails {

        public string path;
        public STPMethod method;
        public Dictionary<string, string> data;

        public string displayText;
        public Color displayColor;

        public static STPRouteDetails from(string displayText, string path, STPMethod method, Dictionary<string, string> data = null, Color? displayColor = null) {
            STPRouteDetails route = new STPRouteDetails();
            route.displayText = displayText;
            route.path = path;
            route.method = method;
            route.data = data;
            route.displayColor = displayColor ?? Color.black;
            return route;
        }

        public bool validate(STPCommand command) {
            bool validateDictionary = true;
            if (command.data != null && data != null) {
                foreach (string key in command.data.Keys)
                    if (!data.ContainsKey(key)) {
                        validateDictionary = false;
                        break;
                    }
            }
            return validateDictionary && method == command.method && command.route == path;
        }
    }
    private static List<STPRouteDetails> _savedRoutes = new List<STPRouteDetails>();
    public static List<STPRouteDetails> routesList {
        get {
            if (_savedRoutes.Count < 1) {
                _initSavedRoutes();
            }
            return _savedRoutes;
        }
    }
    public static List<STPRouteDetails> routesListNoDuplicates {
        get {
            return routesList
            .GroupBy(route => route.path)
            .Select(group => group.First())
            .ToList();
        }
    }

    private static void _initSavedRoutes() {

        Dictionary<string, string> db_delete_data = new Dictionary<string, string>();
        db_delete_data.Add("id", "");

        Dictionary<string, string> electromagnet_power_data = new Dictionary<string, string>();
        electromagnet_power_data.Add("electromagnet_power", "");

        Dictionary<string, string> db_post_data = new Dictionary<string, string>();
        db_post_data.Add("electromagnet", "");
        db_post_data.Add("hall_sensor", "");
        db_post_data.Add("electromagnet_power", "");
        db_post_data.Add("actions", "");

        Dictionary<string, string> db_put_or_save_data = new Dictionary<string, string>();
        db_put_or_save_data.Add("id", "");
        db_put_or_save_data.Add("electromagnet", "");
        db_put_or_save_data.Add("hall_sensor", "");
        db_put_or_save_data.Add("electromagnet_power", "");
        db_put_or_save_data.Add("actions", "");

        _savedRoutes = new List<STPRouteDetails>();

        _savedRoutes.Add(STPRouteDetails.from("API Help", "/", STPMethod.GET));
        _savedRoutes.Add(STPRouteDetails.from("Database", "/db/", STPMethod.GET, null, Color.red));
        //_savedRoutes.Add(STPRouteDetails.from("Database", "/db/", STPMethod.POST, db_post_data, Color.red));
        _savedRoutes.Add(STPRouteDetails.from("Database", "/db/", STPMethod.PUT, db_put_or_save_data, Color.red));
        _savedRoutes.Add(STPRouteDetails.from("Database", "/db/", STPMethod.DELETE, db_delete_data, Color.red));
        _savedRoutes.Add(STPRouteDetails.from("List routes", "/device/routes/", STPMethod.GET));
        _savedRoutes.Add(STPRouteDetails.from("Calibrate device", "/device/calibrate/", STPMethod.GET, null, Color.magenta));
        _savedRoutes.Add(STPRouteDetails.from("Enable device", "/device/enable/", STPMethod.PUT, null, Color.blue));
        _savedRoutes.Add(STPRouteDetails.from("Disable device", "/device/disable/", STPMethod.PUT, null, Color.blue));
        _savedRoutes.Add(STPRouteDetails.from("Get status", "/device/data/", STPMethod.GET));
        _savedRoutes.Add(STPRouteDetails.from("Edit settings", "/device/save/", STPMethod.PUT, db_put_or_save_data, Color.red));
        _savedRoutes.Add(STPRouteDetails.from("Get EM strength", "/device/electromagnet/", STPMethod.GET));
        _savedRoutes.Add(STPRouteDetails.from("Set EM strength", "/device/electromagnet/power/", STPMethod.PUT, electromagnet_power_data, Color.red));
        _savedRoutes.Add(STPRouteDetails.from("Get reading", "/device/hallsensor/", STPMethod.GET));
        _savedRoutes.Add(STPRouteDetails.from("Get normalized reading", "/device/hallsensor/normalized/", STPMethod.GET));

    }


}