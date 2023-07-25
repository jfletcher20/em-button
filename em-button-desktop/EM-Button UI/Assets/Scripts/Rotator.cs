using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rotator : MonoBehaviour {
    public Vector3 rotation = new Vector3(0, 0, -0.5f);

    // Update is called once per frame
    void FixedUpdate() {
        transform.Rotate(rotation);
    }
}
