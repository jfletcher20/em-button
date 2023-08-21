using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ActionsListForm : MonoBehaviour {

    public List<EmbAction> embActions = new List<EmbAction>(3);
    public MonitorEvents monitorEvents;

    private void Awake() {
        embActions = monitorEvents.embData.actions;
    }

    public void UpdateEmbActions() {
        var actionsList = GetComponentsInChildren<ActionInputField>();
        embActions.Clear();
        foreach (var action in actionsList) {
            embActions.Add(action.action);
        }
    }

    private void Update() {
        UpdateEmbActions();
    }

}
