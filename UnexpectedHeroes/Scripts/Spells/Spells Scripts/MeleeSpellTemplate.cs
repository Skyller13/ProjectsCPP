using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEditor.Rendering;
using UnityEngine;

public class MeleeSpellTemplate : MonoBehaviour
{
    GameManager manager;
    private Spell spell;
    SpellData data;
    Node currentNode;
    private float timer;
    private float maxTime;

    bool wasCast;

    void Start()
    {
        timer = 0f;
        maxTime = GetComponent<ParticleSystem>().main.duration;

        manager = Toolbox.Instance.GetManager<GameManager>();

        spell = GetComponent<Spell>();
        data = spell.spellData;
        currentNode = spell.CurrentUnit.GetCurrentNode();

        transform.parent = spell.CurrentUnit.gameObject.transform;

        transform.position =
            new Vector3(transform.position.x,
                0.1f,
            transform.position.z);

        transform.rotation = spell.CurrentUnit.transform.rotation;

        List<Unit> targetList = new List<Unit>();
        Node next = currentNode.GetNeighbor((Node.NeighborsEnum)GetComponent<Spell>().CurrentUnit.thisDirection);

        //LINE
        if (data.RangeAction == SpellData.SpellAction.Line)
        {
            int count = 0;

            Toolbox.Instance.GetManager<SoundManager>().Play(data.audioClipCode[0], gameObject);

            for (int i = 0; i < data.range; i++)
            {
                if (next.UnitOnThis != null
                && !targetList.Contains(next.UnitOnThis))
                {
                    targetList.Add(next.UnitOnThis);
                    count++;
                }

                if (count == data.nbTargetRequired
                && data.nbTargetRequired != 0)
                {
                    i = data.range;
                }

                next = next.GetNeighbor((Node.NeighborsEnum)GetComponent<Spell>().CurrentUnit.thisDirection);
            }
        }
        else if (data.RangeAction == SpellData.SpellAction.Cone) //CONE
        {
            //switch to large range after 

            Toolbox.Instance.GetManager<SoundManager>().Play(data.audioClipCode[0], gameObject);

            Node front = next.GetNeighbor((Node.NeighborsEnum)GetComponent<Spell>().CurrentUnit.thisDirection);
            Node left = front.GetNeighbor(Node.NeighborsEnum.Left);
            Node right = front.GetNeighbor(Node.NeighborsEnum.Right);

            if (next.UnitOnThis
            && !targetList.Contains(next.UnitOnThis))
            {
                targetList.Add(next.UnitOnThis);
            }

            if (front.UnitOnThis
            && !targetList.Contains(front.UnitOnThis))
            {
                targetList.Add(front.UnitOnThis);
            }

            if (left.UnitOnThis
                && !targetList.Contains(left.UnitOnThis))
            {
                targetList.Add(left.UnitOnThis);
            }

            if (right.UnitOnThis
                && !targetList.Contains(right.UnitOnThis))
            {
                targetList.Add(right.UnitOnThis);
            }
        }
        else if (data.RangeAction == SpellData.SpellAction.Near) //Around
        {
            Toolbox.Instance.GetManager<SoundManager>().Play(data.audioClipCode[0], gameObject);

            for (int i = 0; i < 8; i++)
            {
                Node newNode = currentNode.GetNeighbor((Node.NeighborsEnum)i);

                if (newNode.UnitOnThis != null)
                {
                    targetList.Add(newNode.UnitOnThis);
                }
            }
        }
        else if (data.RangeAction == SpellData.SpellAction.Area) //Area From Monster To Target
        {
            List<Node> distanceList = new List<Node>();

            Toolbox.Instance.GetManager<SoundManager>().Play(data.audioClipCode[0], gameObject);

            distanceList = Toolbox.Instance.GetManager<PathFindingManager>().GetRange(currentNode, data.range);

            //check range
            foreach (var node in distanceList)
            {
                if (node.UnitOnThis != null
                    && node.UnitOnThis != currentNode.UnitOnThis
                    && !targetList.Contains(node.UnitOnThis))
                {
                    targetList.Add(node.UnitOnThis);
                }
            }
        }

        if (targetList.Count > 0)
        {
            Toolbox.Instance.GetManager<SpellManager>().DealDamage(targetList, data, GetComponent<Spell>().CurrentUnit);

            //delete this when useless
            Debug.Log(data.damage);
            Debug.Log(targetList.Count);
        }
    }

    private void Update()
    {
        timer += Time.deltaTime;

        if (timer >= maxTime)
        {
            if (spell.CurrentUnit.typeUnit == Unit.TypeUnit.Player)
            {
                spell.CurrentUnit.GetComponent<PlayerController>().CallEndAction();
            }

            Toolbox.Instance.GetManager<GameManager>().IncrementTurn();

            Destroy(this.gameObject);
        }
    }
}
