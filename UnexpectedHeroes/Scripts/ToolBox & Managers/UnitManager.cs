using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnitPool = ObjectPool<Unit>;

public class UnitManager : MonoBehaviour
{
    public Unit Player;
    public List<Unit> Allies = new List<Unit>();
    public List<Unit> EnemyUnitPool = new List<Unit>();


    private void Awake()
    {
       
        
       
    }

    // Start is called before the first frame update
    void Start()
    {

        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
