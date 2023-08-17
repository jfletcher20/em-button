using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using System.Linq;

public class SettingsTab : MonoBehaviour {
    public SerialMonitor serialMonitor;

    public Button settingsButton;
    public GameObject settingsMenu;
    private Animator settingsMenuAnimator;

    public Button sendCommandButton;

    public Button cancelButton;
    public Button submitDataButton;
    public GameObject dataForm;
    private Animator dataFormAnimator;

    public GameObject routeNavigator;
    public GameObject routeSelectablePrefab;

    public GameObject embFormParent;
    private TMP_InputField[] embFormFields;
    private Slider embFormSlider;
    public TMP_InputField singleInputField;

    public MouseInteractions embModelClickInteractions;

    private MonitorEvents monitorEvents;
    private SelectedRouteManager selectedRouteManager;

    private void Awake() {
        monitorEvents = GetComponent<MonitorEvents>();
        selectedRouteManager = GetComponent<SelectedRouteManager>();
        settingsMenuAnimator = settingsMenu.GetComponent<Animator>();
        dataFormAnimator = dataForm.transform.parent.GetComponent<Animator>();
        embFormFields = embFormParent.GetComponentsInChildren<TMP_InputField>();
        embFormSlider = embFormParent.GetComponentInChildren<Slider>();
        clearChildren();
    }

    void Start() {
        settingsButton.onClick.AddListener(animateSettingsMenu);
        settingsButton.onClick.AddListener(initRoutes);
        sendCommandButton.onClick.AddListener(_loadRoute);
        cancelButton.onClick.AddListener(_hideDataForm);
        submitDataButton.onClick.AddListener(_loadRouteWithData);
    }

    void animateSettingsMenu() {
        settingsMenuAnimator.SetBool("enabled", !settingsMenuAnimator.GetBool("enabled"));
        embModelClickInteractions.disabled = !embModelClickInteractions.disabled;
    }

    private bool _initRoutes = true;
    public void initRoutes() {
        if (!_initRoutes) return;
        else _initRoutes = false;
        clearChildren();
        foreach (RouteManagement.STPRouteDetails route in RouteManagement.routesListNoDuplicates) {
            Selectable option = Instantiate(routeSelectablePrefab, routeNavigator.transform).GetComponent<Selectable>();
            TextMeshProUGUI text = option.GetComponentInChildren<TextMeshProUGUI>();
            text.color = route.displayColor;
            text.text = route.displayText;
        }
        selectedRouteManager.Initialize(routeNavigator.GetComponentsInChildren<Selectable>().ToList());
    }

    private void _loadRoute() {
        RouteManagement.STPRouteDetails route = selectedRouteManager.getSelectedRoute();
        STPCommand command = new STPCommand { route = route.path, method = route.method, data = route.data };
        if (route.validate(command) && route.data == null) {
            dataFormAnimator.SetBool("enabled", false);
            serialMonitor.sendCommand(command);
        } else {
            dataFormAnimator.SetBool("enabled", true);
            sendCommandButton.enabled = false;
            foreach (Transform child in dataForm.transform)
                child.gameObject.SetActive(false);
            if (route.method != STPMethod.DELETE && (route.path == "/db/" || route.path == "/device/save/")) {
                embFormParent.SetActive(true);
                embFormFields[0].text = monitorEvents.embData.hall_sensor.ToString();
                embFormFields[1].text = monitorEvents.embData.electromagnet.ToString();
                embFormSlider.value = (float)monitorEvents.embData.electromagnet_power;
            } else {
                singleInputField.transform.parent.gameObject.SetActive(true);
            }
        }
    }

    private void _hideDataForm() {
        dataFormAnimator.SetBool("enabled", false);
        sendCommandButton.enabled = true;
    }

    private void _loadRouteWithData() {
        RouteManagement.STPRouteDetails route = selectedRouteManager.getSelectedRoute();
        if (route.data == null) return;
        List<string> keys = route.data.Keys.ToList();
        STPCommand command = new STPCommand { route = route.path, method = route.method };
        switch (route.path) {
            case "/db/":
                switch(route.method) {
                    case STPMethod.POST:
                    case STPMethod.PUT:
                        for (int i = 0; i < keys.Count && i < embFormFields.Length - 1; i++) {
                            print(keys[i] + ": " + i.ToString());
                            command.data.Add(keys[i], embFormFields[i].text);
                        }
                        break;
                    case STPMethod.DELETE:
                        command.data.Add(keys.First(), singleInputField.text);
                        break;
                }
                break;
            case "/device/save":
                if (route.method == STPMethod.PUT)
                    for (int i = 0; i < keys.Count && i < embFormFields.Length; i++)
                        command.data.Add(keys[i], embFormFields[i].text);
                break;
            case "/device/electromagnet/power/":
                if (route.method == STPMethod.PUT)
                    command.data.Add(keys.First(), singleInputField.text);
                break;
        }
        _hideDataForm();
    }

    public void clearChildren() {
        foreach (Transform child in routeNavigator.transform) {
            Destroy(child.gameObject);
        }
    }

}
