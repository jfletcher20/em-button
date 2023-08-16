using System.Collections.Generic;
using UnityEngine;

public abstract class ISelect : MonoBehaviour {
    public List<Selectable> selectableObjects;
    public abstract void Select(Selectable selectable);
}
