using System.Linq;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Kapy.Statistique;
using Kapy.Move;
using System;


public delegate void FloatDelegate(float _qt);
public class Unit : MonoBehaviour
{
    public enum TypeUnit
    {
        Player,
        Ally,
        Enemy
    }

    public enum StateUnit
    {
        IDLE,
        WALK,
        DEATH,
        VICTORY
    }

    public TypeUnit typeUnit = TypeUnit.Enemy;
    public StateUnit stateUnit = StateUnit.IDLE;
    public string nameCode = "Unit_NoName";

    public bool IsAlive = true;
    public bool IsInCombat = false;

    public SpellData.SpellElement typeElement1 = SpellData.SpellElement.Earth;
    public SpellData.SpellElement typeElement2 = SpellData.SpellElement.Earth;

    List<TemporaryEffet> temporaryEffects = new List<TemporaryEffet>();


    #region Stat
    protected float statCap = 500f;
    protected float baseDefense = 0.05f;
    protected float baseResistance = 0.05f;
    protected float maxPercentageDefRes = 0.7f;

    [SerializeField] KapyStatistique StatNative;
    [SerializeField] KapyStatistique StatCurrent;


    public float HealthCurrent = 100f;
    public float ManaCurrent = 100f;
    #endregion

    #region Events
    public event VoidDelegate ONUnitDeath;
    public event FloatDelegate ONDamageTaken;
    public event VoidDelegate ONCacAttack;
    public event VoidDelegate ONCastAttack;
    public event FloatDelegate ONHealRecieved;
    public event VoidDelegate ONLowHealth;
    public event VoidDelegate ONLowMana;
    public event VoidDelegate ONOutofMana;
    #endregion

    #region Position, Movement & Node reference
    public typeDirection thisDirection;

    [SerializeField] private Node CurrentNodePos;

    public List<Node> path;

    public int PosX = 0;
    public int PosY = 0;

    bool shouldLockPosToNode = true;
    #endregion

    #region Reference to Managers
    protected GameManager gameManager;
    protected PathFindingManager pathFindingManager;
    #endregion


    virtual protected void CustomAwake()
    {
       
    }

    virtual protected void CustomStart()
    {
        gameManager = Toolbox.Instance.GetManager<GameManager>();
        pathFindingManager = Toolbox.Instance.GetManager<PathFindingManager>();

        gameManager.ONEndMacroTurn += UpdateTemporaryEffects;

        gameManager.ONStartMicroTurn += UpdateStatisticsWithBuffDebuff;

        UpdateUnitPosByNode();

        ONDamageTaken += CallUIFloatingText;
    }

    virtual protected void CustomUpdate()
    {
        KillTemporaryEffects();


        LifeLimitAndDeathCondition();
        ManaLimitCondition();

        UpdateUnitDirection();
        UpdateUnitPosByNode();

        if (CurrentNodePos == null)
        {
            CurrentNodePos = pathFindingManager.GiveNode();
        }
    }

    virtual protected void CustomOnDestroy()
    {
        gameManager.ONEndMacroTurn -= UpdateTemporaryEffects;

        gameManager.ONStartMicroTurn -= UpdateStatisticsWithBuffDebuff;

        ONDamageTaken -= CallUIFloatingText;
    }


    #region Action & skip turn
    virtual public void Action()
    {
        StartCoroutine(TimerAction());
    }

    IEnumerator TimerAction()
    {
        yield return new WaitForSeconds(0.3f);
        gameManager.IncrementTurn();
    }
    #endregion

    #region Stats Management
    void UpdateStats()
    {
        StatCurrent.HealthMax = CheckCapStat(StatNative.HealthMax);
        StatCurrent.HealthRegen = CheckCapStat(StatNative.HealthRegen);
        StatCurrent.ManaMax = CheckCapStat(StatNative.ManaMax);
        StatCurrent.ManaRegen = CheckCapStat(StatNative.ManaRegen);
        StatCurrent.Strength = CheckCapStat(StatNative.Strength);
        StatCurrent.Defense = CheckCapStat(StatNative.Defense);
        StatCurrent.Magic = CheckCapStat(StatNative.Magic);
        StatCurrent.Resistance = CheckCapStat(StatNative.Resistance);
        StatCurrent.Movement = CheckCapStat(StatNative.Movement);
    }

    float CheckCapStat(float _stat)
    {
        if (_stat <= statCap)
        {
            return _stat;
        }
        else
        {
            return statCap;
        }
    }

    float CalculateDefPercentage(float _current)
    {
       return baseDefense + maxPercentageDefRes * (_current / statCap);
    }

    float CalculateResPercentage(float _current)
    {
        return baseResistance + maxPercentageDefRes * (_current / statCap);
    }

    //This is the function that calculate the final stats used in the gameplay
    public float GetFinalStat(TypeStatistic _WhichStat)
    {
        switch (_WhichStat)
        {
            case TypeStatistic.HEALTH:
                return StatCurrent.HealthMax;
            case TypeStatistic.HEALTHREGEN:
                return StatCurrent.HealthRegen;
            case TypeStatistic.MANA:
                return StatCurrent.ManaMax;
            case TypeStatistic.MANAREGEN:
                return StatCurrent.ManaRegen;
            case TypeStatistic.STRENGTH:
                return StatCurrent.Strength;
            case TypeStatistic.DEFENSE:
                return CalculateDefPercentage(StatCurrent.Defense);
            case TypeStatistic.MAGIC:
                return StatCurrent.Magic;
            case TypeStatistic.RESISTANCE:
                return CalculateResPercentage(StatCurrent.Resistance);
            case TypeStatistic.MOVEMENT:
                return StatCurrent.Movement;
            default:
                return 0;
        }
    }

    public KapyStatistique GetFinalStat()
    {
        KapyStatistique temp = new KapyStatistique();
        temp.HealthMax = StatCurrent.HealthMax;
        temp.HealthRegen = StatCurrent.HealthRegen;
        temp.ManaMax = StatCurrent.ManaMax;
        temp.ManaRegen = StatCurrent.ManaRegen;
        temp.Strength = StatCurrent.Strength;
        temp.Defense = CalculateDefPercentage(StatCurrent.Defense);
        temp.Magic = StatCurrent.Magic;
        temp.Resistance = CalculateResPercentage(StatCurrent.Resistance);
        temp.Movement = StatCurrent.Movement;

        return temp;
    }
    #endregion

    #region Damage received management
    public void DoDamageOrHeal(Damage _damage)
    {
        float resultDamage = 0;
        List<ForceShield> listShields = new List<ForceShield>();

        foreach (var item in temporaryEffects)
        {
            ForceShield shield = item as ForceShield;
            if (shield != null)
            {
                listShields.Add(shield);
            }
        }

        resultDamage = Damage.ReturnDamage(_damage, typeElement1, typeElement2);
        foreach (var item in listShields)
        {
            if (resultDamage <= 0f)
            {
                break;
            }
            else
            {
                resultDamage = item.DoDamageToShield(resultDamage);
            }
        }

        float dmg = 0f;
        switch (_damage.typeDMG)
        {
            case SpellData.Category.Physical:
                dmg = Mathf.Round((resultDamage * (1f - GetFinalStat(TypeStatistic.DEFENSE))));
                HealthCurrent -= dmg;
                ONDamageTaken?.Invoke(dmg);
                break;
            case SpellData.Category.Magical:
                dmg = Mathf.Round((resultDamage * (1f - GetFinalStat(TypeStatistic.RESISTANCE))));
                HealthCurrent -= dmg;
                ONDamageTaken?.Invoke(dmg);
                break;
            case SpellData.Category.Imperatif:
                ONDamageTaken?.Invoke(resultDamage);
                DoTrueDamage(resultDamage);
                break;
            case SpellData.Category.HealLife:
                ONHealRecieved?.Invoke(Mathf.Round(_damage.value));
                HealthCurrent += Mathf.Round(_damage.value);
                break;

            case SpellData.Category.HealMana:
                ONHealRecieved?.Invoke(Mathf.Round(_damage.value));
                ManaCurrent += Mathf.Round(_damage.value);
                break;
            default:
                Debug.LogError("Error Damage to Unit: " + gameObject.name + " not typed !");
                break;
        }
       
    }

    public void DoTrueDamage(float _qtDamage)
    {
        if (_qtDamage <= HealthCurrent)
        {
            HealthCurrent -= Mathf.Round(_qtDamage);
        }
        else
        {
            HealthCurrent = 0f;
        }
    }


    public void DoDamageOrHealMana(float _value, bool _isItDamage)
    {
        if (_isItDamage)
        {
            ManaCurrent -= Mathf.Round(_value);
        }
        else
        {
            ManaCurrent += Mathf.Round(_value);
        }
    }
    #endregion

    #region Life&Death Conditions
    void LifeLimitAndDeathCondition()
    {
        float finalHealth = GetFinalStat(TypeStatistic.HEALTH);

        if ((HealthCurrent/finalHealth) <= 0.2f)
        {
            ONLowHealth?.Invoke();
        }

        if (HealthCurrent > finalHealth)
        {
            HealthCurrent = finalHealth;
        }
        if (HealthCurrent <= 0f)
        {
            HealthCurrent = 0f;
            stateUnit = StateUnit.DEATH;
            IsAlive = false;
            ONUnitDeath?.Invoke();
        }
        else
        {
            IsAlive = true;
        }
    }

    void ManaLimitCondition()
    {
        float finalMana = GetFinalStat(TypeStatistic.MANA);

        if ((ManaCurrent / finalMana) <= 0.2f)
        {
            ONLowMana?.Invoke();
        }

        if (ManaCurrent > finalMana)
        {
            ManaCurrent = finalMana;
        }
        if (ManaCurrent < 0f)
        {
            ManaCurrent = 0f;
            ONOutofMana?.Invoke();
        }
    }
    #endregion

    #region Temporary Effects management

    void UpdateStatisticsWithBuffDebuff()
    {
        UpdateStats();

        foreach (TemporaryEffet Teffect in temporaryEffects)
        {
            BuffDebuff buff = Teffect as BuffDebuff;
            if (buff != null)
            {
                switch (buff.typeStatistic)
                {
                    case TypeStatistic.HEALTH:
                        StatCurrent.HealthMax += buff.value;
                        break;
                    case TypeStatistic.HEALTHREGEN:
                        StatCurrent.HealthRegen += buff.value;
                        break;
                    case TypeStatistic.MANA:
                        StatCurrent.ManaMax += buff.value;
                        break;
                    case TypeStatistic.MANAREGEN:
                        StatCurrent.ManaRegen += buff.value;
                        break;
                    case TypeStatistic.STRENGTH:
                        StatCurrent.Strength += buff.value;
                        break;
                    case TypeStatistic.DEFENSE:
                        StatCurrent.Defense += buff.value;
                        break;
                    case TypeStatistic.MAGIC:
                        StatCurrent.Magic += buff.value;
                        break;
                    case TypeStatistic.RESISTANCE:
                        StatCurrent.Resistance += buff.value;
                        break;
                    case TypeStatistic.MOVEMENT:
                        StatCurrent.Movement += buff.value;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    protected void UpdateTemporaryEffects()
    {
        UpdateStats();

        foreach (TemporaryEffet Teffect in temporaryEffects)
        {
            DamageHealOverTime dot = Teffect as DamageHealOverTime;
            if (dot != null)
            {
                DoDamageOrHeal(dot.tickDamage);
            }

            if (Teffect.Tick() == false)
            {
                Teffect.Alive = false;
            }
        }
    }

    void KillTemporaryEffects()
    {
        bool check = true;
        while (check)
        {
            check = false;
            foreach (TemporaryEffet item in temporaryEffects)
            {
                if (!item.Alive)
                {
                    temporaryEffects.Remove(item);
                    check = true;
                    break;
                }
            }
        }
    }

    public void CreateBuffDebuffEffect(TypeStatistic _whatStat, float _value, int _duration)
    {
        BuffDebuff effect = new BuffDebuff(_whatStat, _value, _duration);
        temporaryEffects.Add(effect);
    }

    public void DestroyAllDebuff()
    {
        List<TemporaryEffet> tmp = temporaryEffects.FindAll(x => x.typeTemporaryEffect == TypeTemporaryEffect.STAT);

        foreach (var item in tmp)
        {
            BuffDebuff debuff = item as BuffDebuff;
            if (debuff != null && debuff.value <= 0f)
            {
                if (temporaryEffects.Contains(debuff))
                {
                    temporaryEffects.Remove(debuff);
                }
            }

        }
    }

    public void CreateDamageHealOverTime(Damage _tickDamage, int _duration, SpellData.DoTEffects statusEffect = SpellData.DoTEffects.Poison)
    {
        DamageHealOverTime DHOT = new DamageHealOverTime(_tickDamage, _duration, statusEffect);
        temporaryEffects.Add(DHOT);
    }

    public void DestroyAllDamageOverTime()
    {
        List<TemporaryEffet> tmp = temporaryEffects.FindAll(x => x.typeTemporaryEffect == TypeTemporaryEffect.DHOT);

        foreach (var item in tmp)
        {
            DamageHealOverTime dmg = item as DamageHealOverTime;
            if (dmg != null && (dmg.tickDamage.typeDMG != SpellData.Category.HealLife && dmg.tickDamage.typeDMG != SpellData.Category.HealMana))
            {
                if (temporaryEffects.Contains(dmg))
                {
                    temporaryEffects.Remove(dmg);
                }
            }
            
        }
    }

    public void CreateStatusEffect(SpellData.Effects _typeStatusEffect, int _duration)
    {
        StatusEffect alt = new StatusEffect(_typeStatusEffect, _duration);
        temporaryEffects.Add(alt);
    }

    public void DestroyAllStatusEffect()
    {
        List<TemporaryEffet> tmp = temporaryEffects.FindAll(x => x.typeTemporaryEffect == TypeTemporaryEffect.CONTROL);

        foreach (var item in tmp)
        {
            StatusEffect control = item as StatusEffect;
            if (control != null)
            {
                if (temporaryEffects.Contains(control))
                {
                    temporaryEffects.Remove(control);
                }
            }

        }
    }

    public void CreateForceShield(float _value, int _duration)
    {
        ForceShield shield = new ForceShield(_value, _duration);
        temporaryEffects.Add(shield);
    }

    //To get the temporary effect list you need
    //Possible types are : BuffDebuff, DamageHealOverTime or StatusEffect
    public List<T> GetTemporaryEffectList<T>() where T : TemporaryEffet
    {
        List<T> list = new List<T>();
        foreach (var item in temporaryEffects)
        {
            T buffer = item as T;
            if (buffer != null)
            {
                list.Add(buffer);
            }
        }

        return list;
    }
    #endregion

    #region Getter/Setter
    public void GetCurrentHP(out float _current, out float _max)
    {
        _current = HealthCurrent;
        _max = StatCurrent.HealthMax;
    }

    public float GetHPPercentage()
    {
        return HealthCurrent / StatCurrent.HealthMax;
    }

    public void GetCurrentMana(out float _current, out float _max)
    {
        _current = ManaCurrent;
        _max = StatCurrent.ManaMax;
    }

    public float GetManaPercentage()
    {
        return ManaCurrent / StatCurrent.ManaMax;
    }

    public Node GetCurrentNode()
    {
        return CurrentNodePos;
    }
    #endregion

    #region Movement function
    public void SetCurrentNodePose(Node _node)
    {
        CurrentNodePos = _node;
    }

    public void MoveUnitToDirection(typeDirection _Direction)
    {
        thisDirection = _Direction;
        switch (_Direction)
        {
            case typeDirection.Up:
                if (CurrentNodePos.GetNeighbor(Node.NeighborsEnum.Up) != null){MoveUnitToNode(CurrentNodePos.GetNeighbor(Node.NeighborsEnum.Up));}
                break;
            case typeDirection.UpRight:
                if (CurrentNodePos.GetNeighbor(Node.NeighborsEnum.UpRight) != null) { MoveUnitToNode(CurrentNodePos.GetNeighbor(Node.NeighborsEnum.UpRight)); }
                break;
            case typeDirection.Right:
                if (CurrentNodePos.GetNeighbor(Node.NeighborsEnum.Right) != null) { MoveUnitToNode(CurrentNodePos.GetNeighbor(Node.NeighborsEnum.Right)); }
                break;
            case typeDirection.DownRight:
                if (CurrentNodePos.GetNeighbor(Node.NeighborsEnum.DownRight) != null) { MoveUnitToNode(CurrentNodePos.GetNeighbor(Node.NeighborsEnum.DownRight)); }
                break;
            case typeDirection.Down:
                if (CurrentNodePos.GetNeighbor(Node.NeighborsEnum.Down) != null) { MoveUnitToNode(CurrentNodePos.GetNeighbor(Node.NeighborsEnum.Down)); }
                break;
            case typeDirection.DownLeft:
                if (CurrentNodePos.GetNeighbor(Node.NeighborsEnum.DownLeft) != null) { MoveUnitToNode(CurrentNodePos.GetNeighbor(Node.NeighborsEnum.DownLeft)); }
                break;
            case typeDirection.Left:
                if (CurrentNodePos.GetNeighbor(Node.NeighborsEnum.Left) != null) { MoveUnitToNode(CurrentNodePos.GetNeighbor(Node.NeighborsEnum.Left)); }
                break;
            case typeDirection.UpLeft:
                if (CurrentNodePos.GetNeighbor(Node.NeighborsEnum.Upleft) != null) { MoveUnitToNode(CurrentNodePos.GetNeighbor(Node.NeighborsEnum.Upleft)); }
                break;
            default:
                break;
        }
    }

   public void MoveUnitToNode(Node _destination)
    {
        //TODO : Add movement points vérification
        if (!_destination.bIsWall && (_destination.UnitOnThis == null || _destination.UnitOnThis == this))
        {
            StartCoroutine(TransitionToNextNode(CurrentNodePos.vPosition, _destination.vPosition)); //Basic Lerp movement

            CurrentNodePos.WipeTheUnit();
            CurrentNodePos = _destination;
            CurrentNodePos.ChangeTheOccupant(this);
        }
    }

    IEnumerator TransitionToNextNode(Vector3 _StartPos, Vector3 _EndPos)
    {
        shouldLockPosToNode = false;
        stateUnit = StateUnit.WALK;
        float timerMax = 0.5f;
        float timer = 0f;

        while(timer < 1)
        {
            yield return 0;
            timer += Time.deltaTime / timerMax;
            transform.position = Vector3.Lerp(_StartPos, _EndPos, timer);
        }
        stateUnit = StateUnit.IDLE;
        shouldLockPosToNode = true;
    }
     
    
    void UpdateUnitDirection()
    {
        switch (thisDirection)
        {
            case typeDirection.Up:
                Vector2 up = new Vector2(0, 1);
                transform.forward = new Vector3(up.x, 0, up.y);
                break;
            case typeDirection.UpRight:
                Vector2 upRight = new Vector2(1, 1);
                transform.forward = new Vector3(upRight.x, 0, upRight.y);
                break;
            case typeDirection.Right:
                Vector2 right = new Vector2(1, 0);
                transform.forward = new Vector3(right.x, 0, right.y);
                break;
            case typeDirection.DownRight:
                Vector2 downRight = new Vector2(1, -1);
                transform.forward = new Vector3(downRight.x, 0, downRight.y);
                break;
            case typeDirection.Down:
                Vector2 down = new Vector2(0, -1);
                transform.forward = new Vector3(down.x, 0, down.y);
                break;
            case typeDirection.DownLeft:
                Vector2 downLeft = new Vector2(-1, -1);
                transform.forward = new Vector3(downLeft.x, 0, downLeft.y);
                break;
            case typeDirection.Left:
                Vector2 left = new Vector2(-1, 0);
                transform.forward = new Vector3(left.x, 0, left.y);
                break;
            case typeDirection.UpLeft:
                Vector2 upLeft = new Vector2(-1, 1);
                transform.forward = new Vector3(upLeft.x, 0, upLeft.y);
                break;
            default:
                break;
        }
    }

    void UpdateUnitPosByNode()
    {
        if (CurrentNodePos != null)
        {
            if(shouldLockPosToNode) transform.position = CurrentNodePos.vPosition;

            PosX = Convert.ToInt32(CurrentNodePos.vPosition.x);
            PosY = Convert.ToInt32(CurrentNodePos.vPosition.z);
        }
    }
    #endregion

    public void BeggingCacAttack()
    {
        ONCacAttack?.Invoke();
    }

    public void BeggingCastAttack()
    {
        ONCastAttack?.Invoke();
    }

    #region Unity root functions
    private void Awake()
    {
        CustomAwake();
    }

    private void Start()
    {
        CustomStart();
    }

    void Update()
    {
        CustomUpdate();
    }

    private void OnDestroy()
    {
        CustomOnDestroy();
    }
    #endregion


    void CallUIFloatingText(float _qt)
    {
        if (_qt > 0f)
        {
            Toolbox.Instance.GetManager<UIManager>().CreateFloatingText((Convert.ToInt32(_qt)).ToString(), transform.position);
        }
    }
}
