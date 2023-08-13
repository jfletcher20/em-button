using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SettingsButton : MonoBehaviour {
	public Button settingsButton;
	public GameObject settingsMenu;
    public SerialMonitor serialMonitor;

    private Animator settingsMenuAnimator;
    private void Awake() {
        settingsMenuAnimator = settingsMenu.GetComponent<Animator>();
    }
    void Start() {
		Button btn = settingsButton.GetComponent<Button>();
        btn.onClick.AddListener(animateSettingsMenu);
        btn.onClick.AddListener(initRoutes);
	}

    private List<string> routes;
    void animateSettingsMenu() {
        settingsMenuAnimator.SetBool("enabled", !settingsMenuAnimator.GetBool("enabled"));
    }

    void initRoutes() {
        STPCommand command = STPCommand.from("/device/routes/", STPMethod.GET);
        serialMonitor.sendCommand(command);
    }

}
