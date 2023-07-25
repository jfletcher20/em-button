using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MouseInputs : MonoBehaviour {
    public bool showCursor = true;
    void Start() {
        Cursor.visible = showCursor;
    }
}
