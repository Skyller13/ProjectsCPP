using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Reflection;
using UnityEngine;

public class Teleportation : MonoBehaviour
{
    private Material DissolveMat;
    private Material SolveInMat;
    private Unit selfUnit;
    private Material[] mats;

    private Node startNode;
    private Node futureNode;
    private Node.NeighborsEnum direction;

    private bool isPlayer = false;

    void Start()
    {
        //instantiate
        DissolveMat = Resources.Load("Materials/DissolveUnit", typeof(Material)) as Material;
        SolveInMat = Resources.Load("Materials/SolveUnit", typeof(Material)) as Material;
        DissolveMat.SetFloat("_Timer", Time.timeSinceLevelLoad);
        SolveInMat.SetFloat("_Timer", Time.timeSinceLevelLoad);

        //unitCreation
        selfUnit = GetComponent<Spell>().CurrentUnit;

        direction = (Node.NeighborsEnum)selfUnit.thisDirection;

        futureNode = selfUnit.GetCurrentNode().GetNeighbor(direction);

        if (futureNode == null
        || futureNode.bIsWall
        || futureNode.UnitOnThis != null)
        {
            Toolbox.Instance.GetManager<GameManager>().IncrementTurn();

            if (selfUnit.typeUnit == Unit.TypeUnit.Player)
            {
                selfUnit.GetComponent<PlayerController>().CallEndAction();
            }

            Destroy(this.gameObject, 0.15f);
            return;
        }

        startNode = selfUnit.GetCurrentNode();

        if (selfUnit.typeUnit == Unit.TypeUnit.Player)
        {
            isPlayer = true;
        }

        for (int i = 0; i < GetComponent<Spell>().spellData.range - 1; i++)
        {
            if (futureNode.GetNeighbor(direction) != null)
            {
                if (!futureNode.GetNeighbor(direction).bIsWall
                    && futureNode.GetNeighbor(direction).UnitOnThis == null)
                {
                    futureNode = futureNode.GetNeighbor(direction);
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }

        if (futureNode != startNode
        && futureNode != null)
        {
            StartCoroutine(DissolveAndMove());
        }
        else
        {
            Toolbox.Instance.GetManager<GameManager>().IncrementTurn();

            if (selfUnit.typeUnit == Unit.TypeUnit.Player)
            {
                selfUnit.GetComponent<PlayerController>().CallEndAction();
            }

            Destroy(this.gameObject, 0.15f);
            return;
        }
    }
    
    IEnumerator DissolveAndMove()
    {
        //Set Material
        int length = selfUnit.gameObject.GetComponentInChildren<SkinnedMeshRenderer>().materials.Length;
        mats = selfUnit.gameObject.GetComponentInChildren<SkinnedMeshRenderer>().materials;

        selfUnit.gameObject.GetComponentInChildren<SkinnedMeshRenderer>().materials = new Material[length];

        var tempMat = selfUnit.gameObject.GetComponentInChildren<SkinnedMeshRenderer>().materials;

        Toolbox.Instance.GetManager<SoundManager>().Play(GetComponent<Spell>().spellData.audioClipCode[0], gameObject);

        for (int i = 0; i < length; i++)
        {
            DissolveMat.SetTexture("_MainTex", mats[i].mainTexture);

            tempMat[i] = new Material(DissolveMat);
        }

        selfUnit.gameObject.GetComponentInChildren<SkinnedMeshRenderer>().materials = tempMat;
        startNode.UnitOnThis = null;
        startNode = null;

        yield return new WaitForSeconds(1f);

        if (futureNode != null)
        {
            selfUnit.SetCurrentNodePose(futureNode);
            selfUnit.gameObject.transform.position = futureNode.vPosition;
            selfUnit.GetCurrentNode().UnitOnThis = selfUnit;
        }

        for (int i = 0; i < length; i++)
        {
            SolveInMat.SetTexture("_MainTex", mats[i].mainTexture);

            tempMat[i] = new Material(SolveInMat);
        }

        selfUnit.gameObject.GetComponentInChildren<SkinnedMeshRenderer>().materials = tempMat;

        yield return new WaitForSeconds(1f);

        if (isPlayer)
        {
           Toolbox.Instance.GetManager<GameManager>().GetPlayerUnit().GetComponent<PlayerController>().CallEndAction();
        }

        selfUnit.gameObject.GetComponentInChildren<SkinnedMeshRenderer>().materials = mats;

        Toolbox.Instance.GetManager<GameManager>().IncrementTurn();

        //set back materials 

        Destroy(this.gameObject, 0.15f);
    }
}
