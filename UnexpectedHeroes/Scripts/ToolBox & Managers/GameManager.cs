using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;

public enum TypeAction
{
    MOVE = 0 ,
    ATTACK = 1,
    ITEM = 2,
    SKIP = 3
}

public enum StateTurn
{
    BEGIN = 0,
    MIDDLE = 1,
    END = 2
}

public class UnitTurn
{
    public StateTurn stepTurn = StateTurn.BEGIN;
    public string unitName = "NoName";
    public int indexOfOrder = 0;
}

#region Delegates
public delegate void VoidDelegate();
public delegate void RefUnitDelegate(string _unitName);  
public delegate void TypeActionDelegate(TypeAction _actionDone);
#endregion

public class GameManager : MonoBehaviour
{
    [SerializeField] StateTurn MacroTurn = StateTurn.BEGIN;
    [SerializeField] StateTurn MicroTurn = StateTurn.BEGIN;

    [SerializeField] Unit Player;
    [SerializeField] List<Unit> Allies = new List<Unit>();
    [SerializeField] List<Unit> Enemies = new List<Unit>();

    [SerializeField] int indexOfOrder = 0;
    [SerializeField] List<Unit> orderInTurn= new List<Unit>();

    public bool IsInCombat = false;
    public bool IsPlayerTurn = false;
    public Unit.TypeUnit CurrentTypeOfTurn = Unit.TypeUnit.Player;

    public float turnSpeed = 0f;
    public bool xbox = false;

    bool IsInit = false;

    string currentTurnStep;

    public int TurnNumber = 0;

    #region Events

    #region Micro turn
    public event VoidDelegate ONStartMicroTurn;
    public event VoidDelegate ONMicroTurnAction;
    public event VoidDelegate ONEndMicroTurn;
    #endregion

    #region Macro turn
    public event VoidDelegate ONStartMacroTurn;
    public event VoidDelegate ONEndMacroTurn;
    #endregion

    #region System
    public event VoidDelegate ONManagersInitialized;
    #endregion

    #region Organisation
    public event VoidDelegate ONCombatBeggin;
    public event VoidDelegate ONCombatEnd;
    #endregion

    #endregion

    bool HasToIncrementTurn = true;

    private void Start()
    {

    }


    // Update is called once per frame
    void Update()
    {
        if (IsInit)
        {
            string[] names = Input.GetJoystickNames();

            for (int i = 0; i < names.Length; i++)
            {
                if (names[i].Length == 33)
                {
                    xbox = true;
                }
            }

            if (HasToIncrementTurn)
            {
                HasToIncrementTurn = false;

                UpdateMacroTurn();
                //Debug
            }

            if (orderInTurn != null && orderInTurn.Count > 0)
            {
                bool temp = false;
                foreach (var item in orderInTurn)
                {
                    temp = (temp || item.IsInCombat);
                }
                if (temp == true && IsInCombat == false)
                {
                    ONCombatBeggin?.Invoke();
                    IsInCombat = true;
                }
                else if (temp == false && IsInCombat == true)
                {
                    ONCombatEnd?.Invoke();
                    IsInCombat = false;
                }
            }
        }        
    }

    void UpdateMacroTurn()
    {
        switch (MacroTurn)
        {
            case StateTurn.BEGIN:
                currentTurnStep = "Macro Start";
                ONStartMacroTurn?.Invoke();
                IncrementMacroTurn();
                TurnNumber++;
                break;
            case StateTurn.MIDDLE:

                UpdateMicroTurn();

                break;
            case StateTurn.END:
                currentTurnStep = "Macro end";
                //Tick Dot
                //Buff Debuff timer decrement
                ONEndMacroTurn?.Invoke();
                EndMacroTurn();
                StartCoroutine(TimerTransition());
                break;
            default:
                break;
        }
    }

    void UpdateMicroTurn()
    {
        switch (MicroTurn)
        {
            case StateTurn.BEGIN:
                currentTurnStep = "Micro start";
                //All unit stats calculation
                ONStartMicroTurn?.Invoke();
                CurrentTypeOfTurn = orderInTurn[indexOfOrder].typeUnit;
                IncrementTurn();

                break;
            case StateTurn.MIDDLE:
                currentTurnStep = "Micro middle > " + orderInTurn[indexOfOrder].gameObject.name;
                
                //This unitTurn Action
                ONMicroTurnAction?.Invoke();
                AskForAction();

                break;
            case StateTurn.END:
                currentTurnStep = "Micro end";
                ONEndMicroTurn?.Invoke();
                IsPlayerTurn = false;
                EndMicroTurn();
                StartCoroutine(TimerTransition());
                break;
            default:
                break;
        }
    }

    void AskForAction()
    {
        switch (orderInTurn[indexOfOrder].typeUnit)
        {
            case Unit.TypeUnit.Player:
                IsPlayerTurn = true;
                //IncrementTurn();
                break;
            case Unit.TypeUnit.Ally:
                orderInTurn[indexOfOrder].Action();
                break;
            case Unit.TypeUnit.Enemy:
                orderInTurn[indexOfOrder].Action();
                break;
            default:
                break;
        }
    }


    IEnumerator TimerTransition()
    {
        HasToIncrementTurn = false;
        yield return new WaitForSeconds(turnSpeed);
        HasToIncrementTurn = true;
    }


    public Unit GetPlayerUnit()
    {
        return Player;
    }

    public List<Unit> GetAllies()
    {
        return Allies;
    }

    public List<Unit> GetEnemies()
    {
        return Enemies;
    }

    #region Donjon floor initialization function
    public void InitDonjonFloor()
    {
        Enemies = FindObjectsOfType<Unit>().Where(x => x.typeUnit == Unit.TypeUnit.Enemy).ToList();
        Allies = FindObjectsOfType<Unit>().Where(x => x.typeUnit == Unit.TypeUnit.Ally).ToList();
        Player = FindObjectsOfType<Unit>().First(x => x.typeUnit == Unit.TypeUnit.Player);


        foreach (var item in Allies)
        {
            orderInTurn.Add(item);
        }
        foreach (var item in Enemies)
        {
            orderInTurn.Add(item);
        }
        orderInTurn.Add(Player);


        orderInTurn.Reverse();
        MicroTurn = StateTurn.BEGIN;
        MacroTurn = StateTurn.BEGIN;

        TurnNumber = 0;
        IsInit = true;

        /////////////////////////////////////////////////////Temp

        Light mainlight = FindObjectsOfType<Light>().First(x => x.type == LightType.Directional);
        if (mainlight != null)
        {
            List<Light> allDir = FindObjectsOfType<Light>().Where(x => x.type == LightType.Directional).ToList();
            foreach (var item in allDir)
            {
                if (item != mainlight)
                {
                    item.enabled = false;
                }
            }
        }

        /////////////////////////////////////////////////////Temp



        ONManagersInitialized?.Invoke();
    }

    public void EndDonjonFloor()
    {
        Allies.Clear();
        Enemies.Clear();
        orderInTurn = null;
    }
    #endregion

    #region Turn management Function
    public void IncrementTurn()
    {
        MicroTurn++;
        StartCoroutine(TimerTransition());
    }

    void IncrementMacroTurn()
    {
        MacroTurn++;
        StartCoroutine(TimerTransition());
    }
    void EndMicroTurn()
    {
        indexOfOrder++;
        MicroTurn = StateTurn.BEGIN;

        if (indexOfOrder >= orderInTurn.Count)
        {
            IncrementMacroTurn();
        }
    }
    void EndMacroTurn()
    {
        indexOfOrder = 0;
        MacroTurn = StateTurn.BEGIN;
    }
    #endregion
}
