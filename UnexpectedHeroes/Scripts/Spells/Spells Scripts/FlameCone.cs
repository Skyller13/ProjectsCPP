using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FlameCone : MonoBehaviour
{
    private Spell spell;
    private int range;
    private List<Unit> targetList = new List<Unit>();
    private Node.NeighborsEnum direction;

    void Start()
    {
        spell = GetComponent<Spell>();
        range = spell.spellData.range;
        direction = (Node.NeighborsEnum)spell.CurrentUnit.thisDirection;

        Quaternion tempQuaternion = transform.rotation;

        tempQuaternion.eulerAngles = spell.CurrentUnit.transform.eulerAngles;

        transform.rotation = tempQuaternion;

        Node currentNode = spell.CurrentUnit.GetCurrentNode(); //Actual
        Node next = currentNode.GetNeighbor(direction); //first one

        Node right = null;
        Node left = null;

        Toolbox.Instance.GetManager<SoundManager>().Play(spell.spellData.audioClipCode[0], gameObject);

        if (next.UnitOnThis != null)
        {
            targetList.Add(next.UnitOnThis);
        }

        //get all unit on range
        for (int i = 0; i < range; i++)
        {
            next = next.GetNeighbor(direction);

            switch (direction)
            {
                case Node.NeighborsEnum.Up:
                    right = next.GetNeighbor(Node.NeighborsEnum.Right);
                    left = next.GetNeighbor(Node.NeighborsEnum.Left);
                    break;

                case Node.NeighborsEnum.Down:
                    right = next.GetNeighbor(Node.NeighborsEnum.Right);
                    left = next.GetNeighbor(Node.NeighborsEnum.Left);
                    break;

                case Node.NeighborsEnum.Right:
                    right = next.GetNeighbor(Node.NeighborsEnum.Up);
                    left = next.GetNeighbor(Node.NeighborsEnum.Down);
                    break;

                case Node.NeighborsEnum.Left:
                    right = next.GetNeighbor(Node.NeighborsEnum.Up);
                    left = next.GetNeighbor(Node.NeighborsEnum.Down);
                    break;

                case Node.NeighborsEnum.UpRight:
                    right = next.GetNeighbor(Node.NeighborsEnum.Up);
                    left = next.GetNeighbor(Node.NeighborsEnum.Right);
                    break;

                case Node.NeighborsEnum.Upleft:
                    right = next.GetNeighbor(Node.NeighborsEnum.Up);
                    left = next.GetNeighbor(Node.NeighborsEnum.Left);
                    break;

                case Node.NeighborsEnum.DownRight:
                    right = next.GetNeighbor(Node.NeighborsEnum.Down);
                    left = next.GetNeighbor(Node.NeighborsEnum.Right);
                    break;

                case Node.NeighborsEnum.DownLeft:
                    right = next.GetNeighbor(Node.NeighborsEnum.Down);
                    left = next.GetNeighbor(Node.NeighborsEnum.Left);
                    break;
            }

            if (right.UnitOnThis != null)
            {
                targetList.Add(right.UnitOnThis);
            }

            if (left.UnitOnThis != null)
            {
                targetList.Add(left.UnitOnThis);
            }

            if (next.UnitOnThis != null)
            {
                targetList.Add(next.UnitOnThis);
            }

            for (int j = 0; j < i + 1; j++)
            {
                switch (direction)
                {
                    case Node.NeighborsEnum.Up:
                        right = right.GetNeighbor(Node.NeighborsEnum.Right);
                        left = left.GetNeighbor(Node.NeighborsEnum.Left);
                        break;

                    case Node.NeighborsEnum.Down:
                        right = right.GetNeighbor(Node.NeighborsEnum.Right);
                        left = left.GetNeighbor(Node.NeighborsEnum.Left);
                        break;

                    case Node.NeighborsEnum.Right:
                        right = right.GetNeighbor(Node.NeighborsEnum.Up);
                        left = left.GetNeighbor(Node.NeighborsEnum.Down);
                        break;

                    case Node.NeighborsEnum.Left:
                        right = right.GetNeighbor(Node.NeighborsEnum.Up);
                        left = left.GetNeighbor(Node.NeighborsEnum.Down);
                        break;

                    case Node.NeighborsEnum.UpRight:
                        right = right.GetNeighbor(Node.NeighborsEnum.Up);
                        left = left.GetNeighbor(Node.NeighborsEnum.Right);
                        break;

                    case Node.NeighborsEnum.Upleft:
                        right = right.GetNeighbor(Node.NeighborsEnum.Up);
                        left = left.GetNeighbor(Node.NeighborsEnum.Left);
                        break;

                    case Node.NeighborsEnum.DownRight:
                        right = right.GetNeighbor(Node.NeighborsEnum.Down);
                        left = left.GetNeighbor(Node.NeighborsEnum.Right);
                        break;

                    case Node.NeighborsEnum.DownLeft:
                        right = right.GetNeighbor(Node.NeighborsEnum.Down);
                        left = left.GetNeighbor(Node.NeighborsEnum.Left);
                        break;
                }

                if (right.UnitOnThis != null)
                {
                    targetList.Add(right.UnitOnThis);
                }

                if (left.UnitOnThis != null)
                {
                    targetList.Add(left.UnitOnThis);
                }
            }
        }

        StartCoroutine(EndTurn());
    }

    IEnumerator EndTurn()
    {
        yield return new WaitForSeconds(2.5f);

        if (spell.CurrentUnit.typeUnit == Unit.TypeUnit.Player)
        {
            spell.CurrentUnit.GetComponent<PlayerController>().CallEndAction();
        }

        Toolbox.Instance.GetManager<SpellManager>().DealDamage(targetList, spell.spellData, spell.CurrentUnit);

        Toolbox.Instance.GetManager<GameManager>().IncrementTurn();

        Destroy(transform.gameObject);
    }

    void Update()
    {

    }
}
