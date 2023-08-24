using UnityEngine;
using System.Collections.Generic;
using System.Linq;

public class SelectedRouteManager : ISelect {

    public void Initialize(List<Selectable> routeObjects) {
        selectableObjects = routeObjects;
        foreach (Selectable route in routeObjects) {
            route.Initialize(this);
        }
        routeObjects[0].Select();
    }

    public override void Select(Selectable selectable) {
        foreach (Selectable route in selectableObjects) {
            if (route == selectable) {
                route.Select();
                RouteManagement.STPRouteDetails details = routeObjectToRouteDetails(route);
                List<STPMethod> viableMethods = new List<STPMethod>();
                RouteManagement.routesList.ForEach((r) => {
                    if (r.path == details.path) viableMethods.Add(r.method);
                });
                GetComponent<SelectedMethodManager>().DisableExcept(viableMethods);
            } else {
                route.Deselect();
                HideDataForm();
            }
        }
    }

    public void HideDataForm() {
        additionalDataForm.GetComponent<Animator>().SetBool("enabled", false);
        GetComponent<APITab>().sendCommandButton.enabled = true;
    }

    /// <summary>
    /// Disables all methods that aren't in the specified method list.
    /// </summary>
    /// <param name="routes">Methods to keep enabled.</param>
    public void DisableExcept(List<RouteManagement.STPRouteDetails> routes) {
        foreach (Selectable route in selectableObjects) {
            if (!routes.Contains(routeObjectToRouteDetails(route))) {
                if (route.isSelected) route.Deselect();
                route.Disable();
                HideDataForm();
            } else {
                route.Enable();
            }
        }
        if (selectableObjects.Find((m) => m.isSelected) == null) {
            selectableObjects[0].Select();
        }
    }

    public RouteManagement.STPRouteDetails routeObjectToRouteDetails(Selectable routeObject) {
        return RouteManagement.routesListNoDuplicates[selectableObjects.IndexOf(routeObject)];
    }

    public RouteManagement.STPRouteDetails getSelectedRoute() {
        RouteManagement.STPRouteDetails route = RouteManagement.routesListNoDuplicates[selectableObjects.IndexOf(selectableObjects.Find((r) => r.isSelected))];
        route.method = GetComponent<SelectedMethodManager>().getSelectedMethod();
        route = RouteManagement.routesList.Where((r) => r.path == route.path && r.method == route.method).FirstOrDefault();
        return route;
    }
}
