using System.Collections;
using System.Collections.Generic;
using UnityEngine;



public class BaseUnitAnimation : MonoBehaviour
{
    protected Unit unit;
    protected Animator anim;

    bool checkGCD = true;
    InputManager IM;

    // Start is called before the first frame update
    void Start()
    {
        unit = GetComponent<Unit>();
        anim = GetComponent<Animator>();

        unit.ONCacAttack += SetCacAnim;
        unit.ONCastAttack += SetCastAnim;
        unit.ONDamageTaken += SetHitAnim;

        IM = Toolbox.Instance.GetManager<InputManager>();
    }

    private void OnDestroy()
    {
        unit.ONCacAttack -= SetCacAnim;
        unit.ONCastAttack -= SetCastAnim;
        unit.ONDamageTaken -= SetHitAnim;
    }

    // Update is called once per frame
    void Update()
    {
        CustomUpdate();
    }

    protected virtual void CustomUpdate()
    {
        SetLoopingAnim(unit.stateUnit);
    }

    void SetLoopingAnim(Unit.StateUnit _state)
    {
        if (checkGCD)
        {
            switch (_state)
            {
                case Unit.StateUnit.IDLE:
                    anim.SetBool("IsWalking", false);
                    break;
                case Unit.StateUnit.WALK:
                    anim.SetBool("IsWalking", true);
                    StartCoroutine(AnimGCD(0));
                    break;
                case Unit.StateUnit.VICTORY:
                    anim.SetBool("IsVictory", true);
                    break;
                default:
                    break;
            }

            
        }
        
    }

    IEnumerator AnimGCD(int _i)
    {
        float timer = 1f;

        checkGCD = false;

        Debug.Log("Gen animGCD n°: " + _i);

        yield return new WaitForSeconds(0.05f);

        if (IM.XBoxControllerInspector.stickLeft.isUsed)
        {
            StartCoroutine(AnimGCD(_i+1));
        }
        else
        {
            checkGCD = true;
        }

    }

    public void SetCacAnim()
    {
        anim.SetTrigger("Cac");
    }

    public void SetCastAnim()
    {
        anim.SetTrigger("Cast");
    }

    public void SetHitAnim(float _qt)
    {
        anim.SetTrigger("Hit");
    }
}
