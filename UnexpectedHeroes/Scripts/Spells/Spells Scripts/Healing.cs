using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Healing : MonoBehaviour
{
    private Spell spell;

    private Unit selfUnit;

    private List<Unit> targetList = new List<Unit>();

    private int turn;
    private int remainTurn;

    void Start()
    {
        spell = GetComponent<Spell>();

        selfUnit = spell.CurrentUnit;

        transform.position = selfUnit.transform.position;

        turn = Toolbox.Instance.GetManager<GameManager>().TurnNumber;
        remainTurn = spell.spellData.timeRemain;

        //Heal self
        targetList.Add(selfUnit);

        StartCoroutine(EndAction());
    }

    IEnumerator EndAction()
    {
        yield return new WaitForSeconds(0.4f);

        Toolbox.Instance.GetManager<SoundManager>().Play(spell.spellData.audioClipCode[0], gameObject);
        Toolbox.Instance.GetManager<SpellManager>().DealDamage(targetList, spell.spellData, selfUnit);

        yield return new WaitForSeconds(1f);

        if (spell.CurrentUnit.typeUnit == Unit.TypeUnit.Player)
        {
            spell.CurrentUnit.GetComponent<PlayerController>().CallEndAction();
        }
    
        Toolbox.Instance.GetManager<GameManager>().IncrementTurn();

        if (remainTurn == 0)
        {
            Destroy(this.gameObject);
        }
    }

    void Update()
    {
        if (remainTurn > 0)
        {
            transform.position = spell.CurrentUnit.transform.position;

           if (turn != Toolbox.Instance.GetManager<GameManager>().TurnNumber)
            {
                turn = Toolbox.Instance.GetManager<GameManager>().TurnNumber;
                remainTurn--;
            }

           if (remainTurn == 0)
            {
                Destroy(this.gameObject);
            }
        }
    }
}
