using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class SelectionQuad : MonoBehaviour
{
    public enum stateSelection
    {
        STANDBY,
        ENEMY,
        DIRECTION,
        HIGHLIGHT,
    }

    public stateSelection state = stateSelection.STANDBY;
    public Node connectedNode = null;
    private Node next;

    Material material;

    // Start is called before the first frame update
    void Start()
    {
        material = GetComponent<MeshRenderer>().material;
        next = connectedNode;
        //gameObject.SetActive(false);
    }

    public void ChangeGrid()
    {
        //if (connectedNode?.UnitOnThis == null)
        //{
        //    state = stateSelection.STANDBY;
        //}

        if (connectedNode?.UnitOnThis != null)
        {
            if (connectedNode.UnitOnThis.typeUnit == Unit.TypeUnit.Player)
            {
                state = stateSelection.HIGHLIGHT;

                CheckDirection(8);
            }
            else if (connectedNode.UnitOnThis.typeUnit == Unit.TypeUnit.Enemy)
            {
                state = stateSelection.ENEMY;
            }
            else
            {
                state = stateSelection.STANDBY;
            }
        }
        else
        {
            state = stateSelection.STANDBY;
        }
    }

    public void CheckDirection(int _range)
    {
        Node next = connectedNode;

        if (next == null)
        {
            return;
        }

        for (int i = 0; i < _range - 1; i++)
        {
            next = next.GetNeighbor((Node.NeighborsEnum)connectedNode.UnitOnThis.thisDirection);

            if (next?.UnitOnThis == null)
            {
                if (next?.selectionQuad != null)
                {
                    next.selectionQuad.state = stateSelection.DIRECTION;
                }
                else
                {
                    return;
                }
            }
            else
            {
                return;
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (!Toolbox.Instance.GetManager<GameManager>().GetPlayerUnit().GetComponent<PlayerController>().GetPreAction())
        {
            material.SetFloat("_Alpha", 0f);
        }
        else
        {
            switch (state)
            {
                case stateSelection.STANDBY:
                    material.SetColor("_MainColor", new Color(0, 0, 1));
                    break;
                case stateSelection.ENEMY:
                    material.SetColor("_MainColor", new Color(1, 0, 0));
                    break;
                case stateSelection.DIRECTION:
                    material.SetColor("_MainColor", new Color(1, 1, 0));
                    break;
                case stateSelection.HIGHLIGHT:
                    material.SetColor("_MainColor", new Color(0, 1, 1));
                    break;
                default:
                    break;
            }

            ChangeGrid();
            material.SetFloat("_Alpha", 0.7f);
        }
    }
}
