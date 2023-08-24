using System.Collections.Generic;
using UnityEngine;

public abstract class ISelect : MonoBehaviour {
    public GameObject additionalDataForm;
    public List<Selectable> selectableObjects;
    public abstract void Select(Selectable selectable);
}
