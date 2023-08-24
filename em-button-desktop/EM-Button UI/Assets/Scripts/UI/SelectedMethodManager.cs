using UnityEngine;
using System.Collections.Generic;

public class SelectedMethodManager : ISelect {

    private List<STPMethod> methods = new List<STPMethod>{ STPMethod.GET, STPMethod.POST, STPMethod.PUT, STPMethod.DELETE };

    private void Start() {
        foreach (Selectable method in selectableObjects) {
            method.Initialize(this);
        }
        selectableObjects[0].Select();
    }

    public override void Select(Selectable selectable) {
        foreach (Selectable method in selectableObjects) {
            if (method == selectable) {
                method.Select();
            } else {
                method.Deselect();
                GetComponent<SelectedRouteManager>().HideDataForm();
            }
        }
    }

    /// <summary>
    /// Disables all methods that aren't in the specified method list.
    /// </summary>
    /// <param name="methods">Methods to keep enabled.</param>
    public void DisableExcept(List<STPMethod> methods) {
        foreach (Selectable method in selectableObjects) {
            if(!methods.Contains(methodObjectToMethod(method))) {
                method.Deselect();
                method.Disable();
                GetComponent<SelectedRouteManager>().HideDataForm();
            } else {
                method.Enable();
            }
        }
        if (selectableObjects.Find((m) => m.isSelected) == null) {
            selectableObjects.Find((m) => methods.Contains(methodObjectToMethod(m))).Select();
        }
    }

    public STPMethod methodObjectToMethod(Selectable methodObject) {
        return methods[selectableObjects.IndexOf(methodObject)];
    } 

    public STPMethod getSelectedMethod() {
        return methods[selectableObjects.IndexOf(selectableObjects.Find((m) => m.isSelected))];
    }
}
