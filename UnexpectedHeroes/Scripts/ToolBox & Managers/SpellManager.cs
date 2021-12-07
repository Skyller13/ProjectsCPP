using System.Collections;
using System.Collections.Generic;
using Kapy.Statistique;
using UnityEditor.Compilation;
using UnityEngine;

public class SpellManager : MonoBehaviour
{
    private Dictionary<string, GameObject> spellDictionary = new Dictionary<string, GameObject>(); //Complete Spell list

    GameManager GM;
    bool IsInit = false;
    //initialization 
    public void KapyStart(GameManager _GM)
    {
        GM = _GM;

        GM.ONManagersInitialized += Init;
    }

    private void OnDestroy()
    {
        GM.ONManagersInitialized -= Init;
    }
    public void Init()
    {
        string pathName = "Spells/";
        Object[] tempObjects = Resources.LoadAll(pathName);

        foreach (var obj in tempObjects)
        {
            if (!spellDictionary.ContainsKey(obj.name))
            {
                spellDictionary.Add(obj.name, obj as GameObject);
            }
        }

        //Debug.Log(spellDictionary.Count);
        IsInit = true;
    }

    //This function return a gameobject from a string (reference)
    public GameObject GetSpell(string _name)
    {
        GameObject tempGameObject;

        if (spellDictionary.ContainsKey(_name))
        {
            tempGameObject = spellDictionary[_name];
        }
        else
        {
            Debug.LogWarning("There is no spell with that name !");
            return null;
        }

        return tempGameObject;
    }

    //apply to targets damage/heal
    public void DealDamage(List<Unit> _targetList, SpellData _data, Unit _from)
    {
        foreach (var target in _targetList)
        {
            TypeStatistic tempStatistic = TypeStatistic.STRENGTH;

            //define on which stat you will hit 
            switch (_data.Caracteristic)
            {
                case SpellData.CaracteristicType.Health:
                    tempStatistic = TypeStatistic.HEALTH;
                    break;

                case SpellData.CaracteristicType.Intelligence:
                    tempStatistic = TypeStatistic.MAGIC;
                    break;

                case SpellData.CaracteristicType.Strength:
                    tempStatistic = TypeStatistic.STRENGTH;
                    break;

                case SpellData.CaracteristicType.Armor:
                    tempStatistic = TypeStatistic.DEFENSE;
                    break;

                case SpellData.CaracteristicType.MagicResist:
                    tempStatistic = TypeStatistic.RESISTANCE;
                    break;

                case SpellData.CaracteristicType.Mana:
                    tempStatistic = TypeStatistic.MANA;
                    break;
            }

            //crit & effects
            float critStrike = Random.Range(0, 100);
            float critValue = 1f;

            if (critStrike <= _data.CriticalStrike)
            {
                critValue = _data.CriticalFactor;
            }

            float effectStrike = Random.Range(0, 100);
            float dotStrike = Random.Range(0, 100);

            if (effectStrike <= _data.SideEffectChance)
            {
                switch (_data.Effect)
                {
                    case SpellData.Effects.Paralysis:
                        target.CreateStatusEffect(SpellData.Effects.Paralysis, _data.nbEffectTurn);
                        break;

                    case SpellData.Effects.Root:
                        target.CreateStatusEffect(SpellData.Effects.Root, _data.nbEffectTurn);
                        break;

                    case SpellData.Effects.Silence:
                        target.CreateStatusEffect(SpellData.Effects.Silence, _data.nbEffectTurn);
                        break;

                    case SpellData.Effects.Stun:
                        target.CreateStatusEffect(SpellData.Effects.Stun, _data.nbEffectTurn);
                        break;

                    case SpellData.Effects.None:
                        break;
                }
            }

            if (dotStrike <= _data.DotEffectChance)
            {
                switch (_data.EfectOnTime)
                {
                    case SpellData.DoTEffects.Bleed:
                        target.CreateDamageHealOverTime(
                            new Damage(_data.damageTick, SpellData.Category.Physical, SpellData.SpellElement.None,
                                SpellData.SpellElement.None), _data.nbDotEffectTurn, SpellData.DoTEffects.Bleed);
                        break;

                    case SpellData.DoTEffects.Burn:
                        target.CreateDamageHealOverTime(
                            new Damage(_data.damageTick, SpellData.Category.Magical, SpellData.SpellElement.Fire,
                                SpellData.SpellElement.None), _data.nbDotEffectTurn, SpellData.DoTEffects.Burn);
                        break;

                    case SpellData.DoTEffects.Corrosion:
                        target.CreateDamageHealOverTime(
                            new Damage(_data.damageTick, SpellData.Category.Imperatif, SpellData.SpellElement.None,
                                SpellData.SpellElement.None), _data.nbDotEffectTurn, SpellData.DoTEffects.Corrosion);
                        break;

                    case SpellData.DoTEffects.Poison:
                        target.CreateDamageHealOverTime(
                            new Damage(_data.damageTick, SpellData.Category.Imperatif, SpellData.SpellElement.None,
                                SpellData.SpellElement.None), _data.nbDotEffectTurn, SpellData.DoTEffects.Poison);
                        break;

                    case SpellData.DoTEffects.None:
                        break;
                }
            }

            float totalDamage = (_data.damage * Random.Range(0.7f, 1f)) +
                          (_data.SpellRatio * _from.GetFinalStat(tempStatistic)) * critValue;

            switch (_data.firstSpellType)
            {
                case SpellData.TypeOfSpell.SpellAttack:

                    if (_data.CategoryType == SpellData.Category.Imperatif)
                    {
                        target.DoTrueDamage(totalDamage);
                    }
                    else
                    {
                        target.DoDamageOrHeal(new Damage(totalDamage, _data.CategoryType, _data.firstElement,
                            _data.secondElement));
                    }
                    break;

                case SpellData.TypeOfSpell.MeleeAttack:

                    if (_data.CategoryType == SpellData.Category.Imperatif)
                    {
                        target.DoTrueDamage(totalDamage);
                    }
                    else
                    {
                        target.DoDamageOrHeal(new Damage(totalDamage, _data.CategoryType, _data.firstElement,
                            _data.secondElement));
                    }
                    break;

                case SpellData.TypeOfSpell.Buff:
                    target.CreateBuffDebuffEffect(tempStatistic, _data.damage, _data.timeRemain);
                    break;

                case SpellData.TypeOfSpell.Debuff:
                    target.CreateBuffDebuffEffect(tempStatistic, _data.damage * -1f, _data.timeRemain);
                    break;

                case SpellData.TypeOfSpell.Defense:
                    target.CreateForceShield(totalDamage, _data.timeRemain);
                    break;

                case SpellData.TypeOfSpell.Detection:
                    break;

                case SpellData.TypeOfSpell.Heal:

                    if (_data.timeRemain > 0)
                    {
                        //over time
                        target.CreateDamageHealOverTime(
                            new Damage(_data.damageTick, _data.CategoryType, _data.firstElement, _data.secondElement),
                            _data.timeRemain, SpellData.DoTEffects.None);
                    }
                    else
                    {
                        //brut 
                        if (_data.CategoryType == SpellData.Category.HealLife)
                        {
                            target.DoDamageOrHeal(new Damage(totalDamage, _data.CategoryType, _data.firstElement,
                                _data.secondElement));
                        }
                        else if (_data.CategoryType == SpellData.Category.HealMana)
                        {
                            target.DoDamageOrHealMana(totalDamage, false);
                        }
                    }

                    break;

                case SpellData.TypeOfSpell.Dispel:
                    break;

                case SpellData.TypeOfSpell.Summoning:
                    break;

                case SpellData.TypeOfSpell.Any:
                    target.DoDamageOrHeal(new Damage(totalDamage, _data.CategoryType, _data.firstElement,
                        _data.secondElement));
                    break;
            }
        }
    }

    public void DealDamage(Unit _target, SpellData _data, Unit _from)
    {
        TypeStatistic tempStatistic = TypeStatistic.STRENGTH;

        //define on which stat you will hit 
        switch (_data.Caracteristic)
        {
            case SpellData.CaracteristicType.Health:
                tempStatistic = TypeStatistic.HEALTH;
                break;

            case SpellData.CaracteristicType.Intelligence:
                tempStatistic = TypeStatistic.MAGIC;
                break;

            case SpellData.CaracteristicType.Strength:
                tempStatistic = TypeStatistic.STRENGTH;
                break;

            case SpellData.CaracteristicType.Armor:
                tempStatistic = TypeStatistic.DEFENSE;
                break;

            case SpellData.CaracteristicType.MagicResist:
                tempStatistic = TypeStatistic.RESISTANCE;
                break;

            case SpellData.CaracteristicType.Mana:
                tempStatistic = TypeStatistic.MANA;
                break;
        }

        //crit & effects
        float critStrike = Random.Range(0, 100);
        float critValue = 1f;

        if (critStrike <= _data.CriticalStrike)
        {
            critValue = _data.CriticalFactor;
        }

        float effectStrike = Random.Range(0, 100);
        float dotStrike = Random.Range(0, 100);

        if (effectStrike <= _data.SideEffectChance)
        {
            switch (_data.Effect)
            {
                case SpellData.Effects.Paralysis:
                    _target.CreateStatusEffect(SpellData.Effects.Paralysis, _data.nbEffectTurn);
                    break;

                case SpellData.Effects.Root:
                    _target.CreateStatusEffect(SpellData.Effects.Root, _data.nbEffectTurn);
                    break;

                case SpellData.Effects.Silence:
                    _target.CreateStatusEffect(SpellData.Effects.Silence, _data.nbEffectTurn);
                    break;

                case SpellData.Effects.Stun:
                    _target.CreateStatusEffect(SpellData.Effects.Stun, _data.nbEffectTurn);
                    break;

                case SpellData.Effects.None:
                    break;
            }
        }

        if (dotStrike <= _data.DotEffectChance)
        {
            switch (_data.EfectOnTime)
            {
                case SpellData.DoTEffects.Bleed:
                    _target.CreateDamageHealOverTime(
                        new Damage(_data.damageTick, SpellData.Category.Physical, SpellData.SpellElement.None,
                            SpellData.SpellElement.None), _data.nbDotEffectTurn, SpellData.DoTEffects.Bleed);
                    break;

                case SpellData.DoTEffects.Burn:
                    _target.CreateDamageHealOverTime(
                        new Damage(_data.damageTick, SpellData.Category.Magical, SpellData.SpellElement.Fire,
                            SpellData.SpellElement.None), _data.nbDotEffectTurn, SpellData.DoTEffects.Burn);
                    break;

                case SpellData.DoTEffects.Corrosion:
                    _target.CreateDamageHealOverTime(
                        new Damage(_data.damageTick, SpellData.Category.Imperatif, SpellData.SpellElement.None,
                            SpellData.SpellElement.None), _data.nbDotEffectTurn, SpellData.DoTEffects.Corrosion);
                    break;

                case SpellData.DoTEffects.Poison:
                    _target.CreateDamageHealOverTime(
                        new Damage(_data.damageTick, SpellData.Category.Imperatif, SpellData.SpellElement.None,
                            SpellData.SpellElement.None), _data.nbDotEffectTurn, SpellData.DoTEffects.Poison);
                    break;

                case SpellData.DoTEffects.None:
                    break;
            }
        }

        float totalDamage = (_data.damage * Random.Range(0.7f, 1f)) +
                      (_data.SpellRatio * _from.GetFinalStat(tempStatistic)) * critValue;

        switch (_data.firstSpellType)
        {
            case SpellData.TypeOfSpell.SpellAttack:

                if (_data.CategoryType == SpellData.Category.Imperatif)
                {
                    _target.DoTrueDamage(totalDamage);
                }
                else
                {
                    _target.DoDamageOrHeal(new Damage(totalDamage, _data.CategoryType, _data.firstElement,
                        _data.secondElement));
                }
                break;

            case SpellData.TypeOfSpell.MeleeAttack:

                if (_data.CategoryType == SpellData.Category.Imperatif)
                {
                    _target.DoTrueDamage(totalDamage);
                }
                else
                {
                    _target.DoDamageOrHeal(new Damage(totalDamage, _data.CategoryType, _data.firstElement,
                        _data.secondElement));
                }
                break;

            case SpellData.TypeOfSpell.Buff:
                _target.CreateBuffDebuffEffect(tempStatistic, _data.damage, _data.timeRemain);
                break;

            case SpellData.TypeOfSpell.Debuff:
                _target.CreateBuffDebuffEffect(tempStatistic, _data.damage * -1f, _data.timeRemain);
                break;

            case SpellData.TypeOfSpell.Defense:
                _target.CreateForceShield(totalDamage, _data.timeRemain);
                break;

            case SpellData.TypeOfSpell.Detection:
                break;

            case SpellData.TypeOfSpell.Heal:

                if (_data.timeRemain > 0)
                {
                    //over time
                    _target.CreateDamageHealOverTime(
                        new Damage(_data.damageTick, _data.CategoryType, _data.firstElement, _data.secondElement),
                        _data.timeRemain, SpellData.DoTEffects.None);
                }
                else
                {
                    //brut 
                    if (_data.CategoryType == SpellData.Category.HealLife)
                    {
                        _target.DoDamageOrHeal(new Damage(totalDamage, _data.CategoryType, _data.firstElement,
                            _data.secondElement));
                    }
                    else if (_data.CategoryType == SpellData.Category.HealMana)
                    {
                        _target.DoDamageOrHealMana(totalDamage, false);
                    }
                }

                break;

            case SpellData.TypeOfSpell.Dispel:
                break;

            case SpellData.TypeOfSpell.Summoning:
                break;

            case SpellData.TypeOfSpell.Any:
                _target.DoDamageOrHeal(new Damage(totalDamage, _data.CategoryType, _data.firstElement,
                    _data.secondElement));
                break;
        }
    }

    private void Update()
    {
        if (IsInit)
        {
            //!!! Attention Tout mettre dans ce if sinon crash du jeu STP ^^
        }
    }
}
