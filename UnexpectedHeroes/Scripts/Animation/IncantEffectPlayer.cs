using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IncantEffectPlayer : MonoBehaviour
{
    public enum typeIncant
    {
        Ice,
        Fire
    }

    
    [SerializeField] GameObject IncantSpawnLeft;
    [SerializeField] GameObject IncantSpawnRight;

    [SerializeField] GameObject IceIncantPrefab;
    [SerializeField] GameObject FireIncantPrefab;
    [SerializeField] GameObject EarthIncantPrefab;
    [SerializeField] GameObject WindIncantPrefab;
    [SerializeField] GameObject LightIncantPrefab;
    [SerializeField] GameObject ShadowIncantPrefab;

    Animator IceAnimatorLeft;
    Animator IceAnimatorRight;

    Animator FireAnimatorLeft;
    Animator FireAnimatorRigth;

    Animator EarthAnimatorLeft;
    Animator EarthAnimatorRigth;

    Animator WindAnimatorLeft;
    Animator WindAnimatorRigth;

    Animator LightAnimatorLeft;
    Animator LightAnimatorRigth;

    Animator ShadowAnimatorLeft;
    Animator ShadowAnimatorRigth;


    // Start is called before the first frame update
    void Start()
    {
        //Water
        GameObject go = Instantiate(IceIncantPrefab, IncantSpawnLeft.transform);
        IceAnimatorLeft = go.GetComponent<Animator>();

        go = Instantiate(IceIncantPrefab, IncantSpawnRight.transform);
        IceAnimatorRight = go.GetComponent<Animator>();

        //Fire
        go = Instantiate(FireIncantPrefab, IncantSpawnLeft.transform);
        FireAnimatorLeft = go.GetComponent<Animator>();

        go = Instantiate(FireIncantPrefab, IncantSpawnRight.transform);
        FireAnimatorRigth = go.GetComponent<Animator>();

        //Earth
        go = Instantiate(EarthIncantPrefab, IncantSpawnLeft.transform);
        EarthAnimatorLeft = go.GetComponent<Animator>();

        go = Instantiate(EarthIncantPrefab, IncantSpawnRight.transform);
        EarthAnimatorRigth = go.GetComponent<Animator>();

        //Wind
        go = Instantiate(WindIncantPrefab, IncantSpawnLeft.transform);
        WindAnimatorLeft = go.GetComponent<Animator>();

        go = Instantiate(WindIncantPrefab, IncantSpawnRight.transform);
        WindAnimatorRigth = go.GetComponent<Animator>();

        //Light
        go = Instantiate(LightIncantPrefab, IncantSpawnLeft.transform);
        LightAnimatorLeft = go.GetComponent<Animator>();

        go = Instantiate(LightIncantPrefab, IncantSpawnRight.transform);
        LightAnimatorRigth = go.GetComponent<Animator>();

        //Shadow
        go = Instantiate(ShadowIncantPrefab, IncantSpawnLeft.transform);
        ShadowAnimatorLeft = go.GetComponent<Animator>();

        go = Instantiate(ShadowIncantPrefab, IncantSpawnRight.transform);
        ShadowAnimatorRigth = go.GetComponent<Animator>();
    }



    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.H))
        {
            StartIncantAnim(SpellData.SpellElement.Fire, SpellData.SpellElement.Darkness, 1f);
            BaseUnitAnimation unitAnim = GetComponent<BaseUnitAnimation>();
            unitAnim.SetCastAnim();
        }
        if (Input.GetKeyDown(KeyCode.J))
        {
            StartIncantAnim(SpellData.SpellElement.Water, SpellData.SpellElement.Light, 1f);
            BaseUnitAnimation unitAnim = GetComponent<BaseUnitAnimation>();
            unitAnim.SetCastAnim();
        }
    }

    public void StartIncantAnim(SpellData.SpellElement _type1, SpellData.SpellElement _type2, float _time)
    {
        switch (_type1)
        {
            case SpellData.SpellElement.Water:
                StartCoroutine(EndIncantAnimTimer(IceAnimatorLeft, _time));
                if (_type2 == SpellData.SpellElement.None)
                {
                    StartCoroutine(EndIncantAnimTimer(IceAnimatorRight, _time));
                }
                break;
            case SpellData.SpellElement.Fire:
                StartCoroutine(EndIncantAnimTimer(FireAnimatorLeft, _time));
                if (_type2 == SpellData.SpellElement.None)
                {
                    StartCoroutine(EndIncantAnimTimer(FireAnimatorRigth, _time));
                }
                break;
            case SpellData.SpellElement.Earth:
                StartCoroutine(EndIncantAnimTimer(EarthAnimatorLeft, _time));
                if (_type2 == SpellData.SpellElement.None)
                {
                    StartCoroutine(EndIncantAnimTimer(EarthAnimatorRigth, _time));
                }
                break;
            case SpellData.SpellElement.Wind:
                StartCoroutine(EndIncantAnimTimer(WindAnimatorLeft, _time));
                if (_type2 == SpellData.SpellElement.None)
                {
                    StartCoroutine(EndIncantAnimTimer(WindAnimatorRigth, _time));
                }
                break;
            case SpellData.SpellElement.Light:
                StartCoroutine(EndIncantAnimTimer(LightAnimatorLeft, _time));
                if (_type2 == SpellData.SpellElement.None)
                {
                    StartCoroutine(EndIncantAnimTimer(LightAnimatorRigth, _time));
                }
                break;
            case SpellData.SpellElement.Darkness:
                StartCoroutine(EndIncantAnimTimer(ShadowAnimatorLeft, _time));
                if (_type2 == SpellData.SpellElement.None)
                {
                    StartCoroutine(EndIncantAnimTimer(ShadowAnimatorRigth, _time));
                }
                break;
            default:
                break;
        }

        if (_type2 != SpellData.SpellElement.None)
        {
            switch (_type2)
            {
                case SpellData.SpellElement.Water:
                    StartCoroutine(EndIncantAnimTimer(IceAnimatorRight, _time));
                   
                    break;
                case SpellData.SpellElement.Fire:
                    StartCoroutine(EndIncantAnimTimer(FireAnimatorRigth, _time));
                    
                    break;
                case SpellData.SpellElement.Earth:
                    StartCoroutine(EndIncantAnimTimer(EarthAnimatorRigth, _time));
                    
                    break;
                case SpellData.SpellElement.Wind:
                    StartCoroutine(EndIncantAnimTimer(WindAnimatorRigth, _time));
                   
                    break;
                case SpellData.SpellElement.Light:
                    StartCoroutine(EndIncantAnimTimer(LightAnimatorRigth, _time));
                    
                    break;
                case SpellData.SpellElement.Darkness:
                    StartCoroutine(EndIncantAnimTimer(ShadowAnimatorRigth, _time));
                    
                    break;
                default:
                    break;
            }
        }
    }

    IEnumerator EndIncantAnimTimer(Animator _anim, float _time)
    {
        bool check = true;
        _anim.SetTrigger("Start");
        while (check)
        {
            yield return 0;
            _time -= Time.deltaTime;
            if (_time <= 0f)
            {
                check = false;
            }
        }

        _anim.SetTrigger("End");
    }
}
