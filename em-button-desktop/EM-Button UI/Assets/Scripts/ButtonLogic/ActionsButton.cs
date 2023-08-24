using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ActionsButton : MonoBehaviour {

    private Button actionsFormButton;
    public Animator actionsFormAnimator;
    public Button cancelButton;
    public Button submitButton;

    private void Awake() {
        actionsFormButton = GetComponentInChildren<Button>();
        actionsFormButton.onClick.AddListener(showActionsForm);
    }

    private void showActionsForm() {
        bool enabled = actionsFormAnimator.GetBool("enabled");
        cancelButton.enabled = submitButton.enabled = enabled;
        actionsFormAnimator.SetBool("enabled", !enabled);
    }

}
