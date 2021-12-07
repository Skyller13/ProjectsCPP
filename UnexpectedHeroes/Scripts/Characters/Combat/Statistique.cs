using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Kapy
{
    namespace Statistique
    {
        public enum TypeStatistic
        {
            HEALTH,
            HEALTHREGEN,
            MANA,
            MANAREGEN,
            STRENGTH,
            DEFENSE,
            MAGIC,
            RESISTANCE,
            MOVEMENT
        }

        [System.Serializable]
        public class KapyStatistique
        {
            public float HealthMax = 100f;
            public float HealthRegen = 1f;

            public float ManaMax = 100f;
            public float ManaRegen = 1f;

            public float Strength = 1f;
            public float Defense = 1f;
            public float Magic = 1f;
            public float Resistance = 1f;
            public float Movement = 1f;
        }

        [System.Serializable]
        public enum TypeTemporaryEffect
        {
            STAT,
            DHOT,
            CONTROL,
            SHIELD,
            NONE
        }

        [System.Serializable]
        public abstract class TemporaryEffet
        {
            public TypeTemporaryEffect typeTemporaryEffect = TypeTemporaryEffect.NONE;
            public int durationMax = 1;
            public int durationCurrent = 1;
            public bool Alive = true;

            public TemporaryEffet()
            {

            }

            public TemporaryEffet(TypeTemporaryEffect _type, int _durationMax)
            {
                typeTemporaryEffect = _type;
                durationMax = durationCurrent = _durationMax;
            }

            public virtual bool Tick()
            {
                durationCurrent--;
                return (durationCurrent > 0) ? true : false;
            }
        }


        [System.Serializable]
        public class BuffDebuff : TemporaryEffet
        {
            public float value = -5f;
            public TypeStatistic typeStatistic = TypeStatistic.HEALTH;

            public BuffDebuff()
            {
                typeTemporaryEffect = TypeTemporaryEffect.STAT;
            }
            
            public BuffDebuff(TypeStatistic _typeStatistic, float _value, int _duration)
            {
                typeTemporaryEffect = TypeTemporaryEffect.STAT;
                typeStatistic = _typeStatistic;
                value = _value;

                durationMax = durationCurrent = _duration;
            }
        }

        [System.Serializable]
        public class DamageHealOverTime : TemporaryEffet
        {
            public Damage tickDamage = new Damage(1f, SpellData.Category.Physical, SpellData.SpellElement.Fire, SpellData.SpellElement.None);
            public SpellData.DoTEffects statusEffect = SpellData.DoTEffects.Poison;

            public DamageHealOverTime()
            {
                typeTemporaryEffect = TypeTemporaryEffect.DHOT;
            }
            public DamageHealOverTime(Damage _tickDamage, int _duration, SpellData.DoTEffects _typeStatusEffect = SpellData.DoTEffects.Poison)
            {
                typeTemporaryEffect = TypeTemporaryEffect.DHOT;
                tickDamage = _tickDamage;
                durationMax = durationCurrent = _duration;
                statusEffect = _typeStatusEffect;
            }
        }

        public class StatusEffect : TemporaryEffet
        {
            public SpellData.Effects typeEffect = SpellData.Effects.Stun;

            public StatusEffect()
            {
                typeTemporaryEffect = TypeTemporaryEffect.CONTROL;
            }

            public StatusEffect(SpellData.Effects _typeEffect, int _duration)
            {
                typeTemporaryEffect = TypeTemporaryEffect.CONTROL;
                typeEffect = _typeEffect;
                durationMax = durationCurrent = _duration;
            }
        }

        public class ForceShield : TemporaryEffet
        {
            public float valueMax = 10f;
            public float valueCurrent = 10f;

            public ForceShield()
            {
                typeTemporaryEffect = TypeTemporaryEffect.SHIELD;
            }

            public ForceShield(float _value, int _duration)
            {
                valueMax = valueCurrent = _value;
                durationMax = durationCurrent = _duration;
            }

            public override bool Tick()
            {
                if(valueCurrent <= 0f)
                {
                    return false;
                }

                return base.Tick();
            }

            public float DoDamageToShield(float _qt)
            {
                if (_qt >= valueCurrent)
                {
                    _qt -= valueCurrent;
                    valueCurrent = 0f;
                    return _qt;
                }
                else
                {
                    valueCurrent -= _qt;
                    return 0f;
                }
            }
        }
    }
}

