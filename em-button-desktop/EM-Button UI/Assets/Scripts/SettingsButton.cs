using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SettingsButton : MonoBehaviour {
	public Button settingsButton;
	public GameObject settingsMenu;
    private Animator settingsMenuAnimator;
    private void Awake() {
        settingsMenuAnimator = settingsMenu.GetComponent<Animator>();
    }
    void Start() {
		Button btn = settingsButton.GetComponent<Button>();
        btn.onClick.AddListener(animateSettingsMenu);
	}


    void animateSettingsMenu() {
        settingsMenuAnimator.SetBool("enabled", !settingsMenuAnimator.GetBool("enabled"));
    }
}
