using System.Collections;
using System.Collections.Generic;
using Kapy.Statistique;
//using System.Security.Policy;
using UnityEditor;
using UnityEngine;
using UnityEngine.UI;

[CreateAssetMenu(fileName = "New Data", menuName = "New Data/Spell")]
public class SpellData : ScriptableObject
{
    public enum TypeOfSpell
    {
        Heal,
        Debuff,
        Buff,
        Dispel,
        MeleeAttack,
        SpellAttack,
        Defense,
        Summoning,
        Detection,
        Any
    }

    public enum CaracteristicType
    {
        Health,
        Intelligence, 
        Strength,
        Mana,
        MagicResist,
        Armor,
    }

    public enum AnimationType
    {
        Distance,
        Melee
    }

    public enum SpellElement
    {
        None = -1,
        Earth = 0,
        Water = 1,
        Wind = 2,
        Fire = 3,
        Light = 4,
        Darkness = 5,
    }

    public enum SpellAction
    {
        Line,
        Cone,
        Near,
        Area,
        Self,
        Target,
        None
    }

    public enum ScriptChoice
    {
        Projectile,
        Instant,
        Melee,
        Shield,
        Cone,
        Heal,
        None,
    }

    public enum Category
    {
        Physical,
        Magical,
        Imperatif,
        HealLife,
        HealMana
    }

    public enum Effects
    {
        Root,
        Paralysis,
        Stun,
        Silence,
        None
    }

    public enum DoTEffects
    {
        Burn,
        Corrosion,
        Poison,
        Bleed,
        None
    }

    public enum TargetNumber
    {
        One,
        Multiple, // precise
        All,
        None 
    }

    public enum TargetType
    {
        Friendly,
        Enemy,
        Any,
        Self,
        None
    }

    public enum TargetSelection
    {
        OneByOne,
        Random,
        None,
    }

        //Infos
    [SerializeField]
    [Header("Informations")]
    public string Name;

    [SerializeField]
    [TextArea]
    public string Description;


    //Spell
    [Header("")]
    public TypeOfSpell firstSpellType;
    public Category CategoryType;
    public CaracteristicType Caracteristic;
    public AnimationType animationType;

    [Header("Target")]
    public TargetType RequiredTarget;
    public TargetNumber NumberTarget;
    public TargetSelection Selection;

    //Elements
    [Header("")]
    public SpellElement firstElement;
    public SpellElement secondElement;
    public Effects Effect;
    public DoTEffects EfectOnTime;

    //Values
    [Header("Values")]
    public int cooldown;
    public float damage;
    [Range(0, 100)]
    public float SideEffectChance;
    public int nbEffectTurn;
    [Range(0, 100)]
    public float DotEffectChance;
    public int nbDotEffectTurn;
    public int damageTick;
    [Range(0, 100)]
    public float CriticalStrike;
    [Range(1, 2.5f)]
    public float CriticalFactor; 
    [Range(0.1f, 2f)]
    public float SpellRatio;
    public int Cost;
    public int levelRequired;
    public float spellSpeed;

    //Range
    [Header("Range")]
    public SpellAction RangeAction;
    public ScriptChoice Condition;
    public int range;
    public int nbTargetRequired;
    public int timeRemain;

    [Header("Prefabs")]
    public GameObject gameobject;

    [Header("Particles")]
    public ParticleSystem particle;

    [Header("Audio")] 
    public string[] audioClipCode;
}
