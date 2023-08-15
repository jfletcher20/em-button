using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class SettingsTab : MonoBehaviour {
    public SerialMonitor serialMonitor;

    public Button settingsButton;
    public GameObject settingsMenu;
    private Animator settingsMenuAnimator;

    public GameObject routeNavigator;
    public Button routeButtonPrefab;

    private void Awake() {
        settingsMenuAnimator = settingsMenu.GetComponent<Animator>();
        clearChildren();
    }

    void Start() {
        Button btn = settingsButton.GetComponent<Button>();
        btn.onClick.AddListener(animateSettingsMenu);
        btn.onClick.AddListener(initRoutes);
    }

    void animateSettingsMenu() {
        settingsMenuAnimator.SetBool("enabled", !settingsMenuAnimator.GetBool("enabled"));
    }

    void initRoutes() {
        STPCommand command = STPCommand.from("/device/routes/", STPMethod.GET);
        serialMonitor.sendCommand(command);
    }

    public void initRoutes(List<string> routes) {
        clearChildren();
        foreach (string route in routes) {
            Button button = Instantiate(routeButtonPrefab, routeNavigator.transform);
            TextMeshProUGUI text = button.GetComponentInChildren<TextMeshProUGUI>();
            _processRouteText(text, route);
            _routeColor(text, route);
            button.onClick.AddListener(() => _loadRoute(route));
        }
    }

    private void _routeColor(TextMeshProUGUI text, string route) {
        if (route.Contains("electromagnet")) {
            text.color = Color.red;
        } else if (route.Contains("hallsensor")) {
            text.color = Color.magenta;
        } else if (route.Contains("device")) {
            text.color = Color.blue;
        } else if (route.Contains("db")) {
            text.color = Color.cyan;
        } else if (route.Contains("enable")) {
            text.color = Color.green;
        } else if (route.Contains("disable")) {
            text.color = Color.red;
        }
    }

    private void _processRouteText(TextMeshProUGUI text, string route) {
        void t(string val) { text.text = val; }
        if (route.Equals("/")) t("Help");
        if (route.Equals("/db/")) t("Database");
        if (route.Equals("/device/routes/")) t("List routes");
        if (route.Equals("/device/calibrate/")) t("Calibrate device");
        if (route.Equals("/device/enable/")) t("Enable device");
        if (route.Equals("/device/disable/")) t("Disable device");
        if (route.Equals("/device/data/")) t("Get status");
        if (route.Equals("/device/save/")) t("Edit settings");
        if (route.Equals("/device/electromagnet/")) t("Get EM strength");
        if (route.Equals("/device/electromagnet/power/")) t("Set EM strength");
        if (route.Equals("/device/hallsensor/")) t("Get reading");
        if (route.Equals("/device/hallsensor/normalized/")) t("Get normalized reading");
    }

    private void _loadRoute(string route) {
        STPCommand command = new STPCommand { route = route, method = STPMethod.GET };
        serialMonitor.sendCommand(command);
    }

    public void clearChildren() {
        foreach (Transform child in routeNavigator.transform) {
            Destroy(child.gameObject);
        }
    }

}
