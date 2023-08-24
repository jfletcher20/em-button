using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class ActionInputField : MonoBehaviour {

    public EmbAction action = new EmbAction();

    private Button duplicateButton, deleteButton;
    public TMP_InputField key;
    public Slider slider;
    private ActionsListForm actionsListForm;
    private void Awake() {
        action.actionId = transform.parent.GetSiblingIndex();
        Button[] buttons = GetComponentsInChildren<Button>();
        duplicateButton = buttons[0];
        deleteButton = buttons[1];
        key = GetComponentInChildren<TMP_InputField>();
        slider = GetComponentInChildren<Slider>();
        actionsListForm = GetComponentInParent<ActionsListForm>();
        duplicateButton.onClick.AddListener(DuplicateAction);
        deleteButton.onClick.AddListener(DeleteAction);
        key.onValueChanged.AddListener(OnKeyValueChanged);
        slider.onValueChanged.AddListener(OnSliderValueChanged);
        action.keyId = 48;
        action.activation_point = (float)((int)(slider.value * 100)) / 100;
    }

    private void OnKeyValueChanged(string newText) {
        if (newText == "") {
            action.keyId = '0';
        } else action.keyId = key.text[0];
    }

    private void OnSliderValueChanged(float newValue) {
        action.activation_point = (float)((int)(newValue * 100)) / 100;
    }

    private void Update() {
        CheckButtonStates();
    }

    private void DuplicateAction() {
        Transform parent = transform.parent;
        Transform newDuplicate = Instantiate(parent, parent.parent);
        newDuplicate.name = parent.name + "_Duplicate";
    }

    private void DeleteAction() {
        Transform parentParent = transform.parent.parent;
        if (parentParent.childCount > 1) {
            actionsListForm.embActions.Remove(action);
            Destroy(transform.parent.gameObject);
        } else {
            Debug.Log("Cannot delete the last child.");
        }
    }

    private void CheckButtonStates() {
        Transform parentParent = transform.parent.parent;
        bool disableButtons = parentParent.childCount >= 3;
        duplicateButton.enabled = !disableButtons;
        deleteButton.enabled = parentParent.childCount > 1;
        action.actionId = transform.parent.GetSiblingIndex();
    }

}
