using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using System.Linq;

public class APITab : MonoBehaviour {
    public SerialMonitor serialMonitor;

    public Button apiButton;
    public GameObject apiMenu;
    private Animator apiMenuAnimator;

    public Button sendCommandButton;

    public Button cancelButton;
    public Button submitDataButton;
    public GameObject dataForm;
    private Animator dataFormAnimator;

    public TMP_InputField commandDataDisplay;

    public GameObject routeNavigator;
    public GameObject routeSelectablePrefab;

    public GameObject emPowerForm;
    public GameObject embFormParent;
    private TMP_InputField[] embFormFields;
    public TMP_InputField deletionConfirmationField;

    public MouseInteractions embModelClickInteractions;

    private MonitorEvents monitorEvents;
    private SelectedRouteManager selectedRouteManager;

    private void Awake() {
        monitorEvents = GetComponent<MonitorEvents>();
        selectedRouteManager = GetComponent<SelectedRouteManager>();
        apiMenuAnimator = apiMenu.GetComponent<Animator>();
        dataFormAnimator = dataForm.transform.parent.GetComponent<Animator>();
        embFormFields = embFormParent.GetComponentsInChildren<TMP_InputField>();
        clearChildren();
    }

    void Start() {
        apiButton.onClick.AddListener(animateSettingsMenu);
        apiButton.onClick.AddListener(initRoutes);
        sendCommandButton.onClick.AddListener(_loadRoute);
        cancelButton.onClick.AddListener(_hideDataForm);
        submitDataButton.onClick.AddListener(_loadRouteWithData);
    }

    void animateSettingsMenu() {
        apiMenuAnimator.SetBool("enabled", !apiMenuAnimator.GetBool("enabled"));
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
        if (!commandDataDisplay.isActiveAndEnabled) commandDataDisplay.gameObject.SetActive(true);
        if (route.validate(command) && route.data == null) {
            dataFormAnimator.SetBool("enabled", false);
            commandDataDisplay.text = command.ToString();
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
                embFormParent.GetComponentInChildren<Slider>().value = (float)monitorEvents.embData.electromagnet_power;
            } else if (route.method == STPMethod.DELETE) {
                deletionConfirmationField.transform.parent.gameObject.SetActive(true);
            } else {
                emPowerForm.GetComponentInChildren<Slider>().value = (float)monitorEvents.embData.electromagnet_power;
                emPowerForm.SetActive(true);
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
        STPCommand command = new STPCommand { route = route.path, method = route.method, data = new Dictionary<string, string>() };
        switch (route.path) {
            case "/db/":
                switch(route.method) {
                    case STPMethod.POST:
                    case STPMethod.PUT:
                        command.data = _getEmbDataFromForm();
                        break;
                    case STPMethod.DELETE:
                        if (!_confirmDeletion()) return;
                        command.data = _getDeletionDataFromForm();
                        break;
                }
                break;
            case "/device/save":
                if (route.method == STPMethod.PUT)
                    command.data = _getEmbDataFromForm();
                print("loaded save route");
                break;
            case "/device/electromagnet/power/":
                if (route.method == STPMethod.PUT)
                    command.data = _getEmDataFromForm();
                break;
        }
        _hideDataForm();
        Debug.LogWarning(command.ToString());
        commandDataDisplay.text = command.ToString();
        serialMonitor.sendCommand(command);
    }

    private bool _confirmDeletion() {
        return deletionConfirmationField.text.ToLower().Equals("yes");
    }

    private Dictionary<string, string> _getDeletionDataFromForm() {
        return new Dictionary<string, string> {
            { "id", monitorEvents.embData.id.ToString() }
        };
    }

    private Dictionary<string, string> _getEmDataFromForm() {
        return new Dictionary<string, string> {
            { "electromagnet_power", ((double)((int)(emPowerForm.GetComponentInChildren<Slider>().value * 100)) / 100).ToString() }
        };
    }

    private Dictionary<string, string> _getEmbDataFromForm() {
        double emPower = (double)((int)(embFormParent.GetComponentInChildren<Slider>().value * 100)) / 100;
        List<EmbAction> embActions = apiMenu.transform.parent.parent.GetComponentInChildren<ActionsListForm>().embActions;
        EmbButton embButton = new EmbButton {
            id = 0,
            hall_sensor = embFormFields[0].text == "" ? 4 : int.Parse(embFormFields[0].text),
            electromagnet = embFormFields[1].text == "" ? 13 : int.Parse(embFormFields[1].text),
            electromagnet_power = emPower,
            actions = embActions,
        };
        return embButton.toStringDictionary();
    }

    public void clearChildren() {
        foreach (Transform child in routeNavigator.transform) {
            Destroy(child.gameObject);
        }
    }

}
