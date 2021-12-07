using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using UnityEngine;
using UnityEngine.UI;
using Kapy.Move;
using Cinemachine;
using Kapy.Statistique;
using UnityEditor;

public class PlayerController : MonoBehaviour
{
    private IngameInputMap ingameMap;
    private TargetInputMap targetMap;
    private InGameMenuMap inGameMenuMap;
    private MenuInGame menuInGame;

    public string[] favoriteSpells = new string[4];
    private readonly int[] spellsCooldown = new int[4];
    private string currentSpell;
    private int currentCD = 0;

    public bool begin = false; //tempList targeting

    private bool diagonal = false;
    private bool canTarget = true;
    private bool isMoving { get; set; }

    //spellCasting
    private int totalNbTarget = 0;
    private int currentNbTarget = 0;

    private int focusTarget = 0;

    private List<Unit> targetList = new List<Unit>();
    private List<Unit> tempList = new List<Unit>();

    private Unit currentUnit;
    private Spell tempSpell;
    private GameObject spellObj;
    private int nbSpell = 0;

    private GameObject highlight; //reference
    private Material highlightMaterial; //reference

    private GameObject highlightObj;
    [SerializeField] private GameObject spawnProjectile;

    public enum currentAction
    {
        isBase, //normal
        isCreating, //createSpell function
        isTargeting, //selection target frame
        isCasting, //Casting a spell
        isMenu, //player is in Menu
        onSpell, //spell is currently used
        endAction, //player did an action => end turn
    }

    public currentAction PlayerAction;

    Unit player;

    private short position;

    private float timerTarget = 0;
    private float timer = 0; //movement

    Vector2 pad;
    private float angle;

    void Start()
    {
        ingameMap = FindObjectOfType<IngameInputMap>();
        targetMap = FindObjectOfType<TargetInputMap>();
        inGameMenuMap = FindObjectOfType<InGameMenuMap>();

        menuInGame = FindObjectOfType<MenuInGame>();

        ///Inputs InGame
        /// 
        //movements
        ingameMap.ONMoveChange += Movements;
        ingameMap.ONDirectionChange += SwitchDirection;

        //actions
        ingameMap.ONBaseAttack += BaseAttack;
        ingameMap.ONFirstSpell += FirstSpell;
        ingameMap.ONSecondSpell += SecondSpell;
        ingameMap.OnThirdSpell += ThirdSpell;
        ingameMap.OnLastSpell += LastSpell;
        ingameMap.OnStart += OpenMenu;
        ///

        /// Inputs Target
        targetMap.ONSelect += SelectEntities;
        targetMap.ONBack += RemoveEntities;
        targetMap.OnAccept += AcceptSelection;
        targetMap.ONCancel += CancelSelection;
        targetMap.ONSwitch += SwitchEntity;
        /// 

        angle = 0;
        pad = new Vector2();
        player = GetComponent<Unit>();
        currentUnit = null;
        currentSpell = "";
        tempSpell = null;
        spellObj = null;
        highlight = Resources.Load("Prefabs/Node_Highlight") as GameObject;
        highlightMaterial = Resources.Load("Materials/selectedTargetMat", typeof(Material)) as Material;
    }

    void OnDestroy()
    {
        ///Inputs In Game
        // movements
        ingameMap.ONMoveChange -= Movements;
        ingameMap.ONDirectionChange -= SwitchDirection;

        //actions
        ingameMap.ONBaseAttack -= BaseAttack;
        ingameMap.ONFirstSpell -= FirstSpell;
        ingameMap.ONSecondSpell -= SecondSpell;
        ingameMap.OnThirdSpell -= ThirdSpell;
        ingameMap.OnLastSpell -= LastSpell;
        ingameMap.OnStart -= OpenMenu;
        ///

        /// Inputs Target
        targetMap.ONSelect -= SelectEntities;
        targetMap.ONBack -= RemoveEntities;
        targetMap.OnAccept -= AcceptSelection;
        targetMap.ONCancel -= CancelSelection;
        targetMap.ONSwitch -= SwitchEntity;
        /// 
    }

    public void OpenMenu()
    {
        if (ingameMap.IM.currentInputMap != inGameMenuMap)
        {
            ingameMap.IM.ChangeInputMap(inGameMenuMap);
            PlayerAction = currentAction.isMenu;

            menuInGame.OpenMenu();
        }
    }

    public bool GetPreAction()
    {
        return ingameMap.isPreAction;
    }

    private void ResetTarget()
    {
        totalNbTarget = 0;
        currentNbTarget = 0;
        focusTarget = 0;
        targetList.Clear();
        tempList.Clear();
        currentUnit = null;
        tempSpell = null;
    }

    private void CamOnPlayer()
    {
        var cam = Camera.main.GetComponent<CinemachineBrain>().ActiveVirtualCamera;

        cam.Follow = player.gameObject.transform;
        cam.LookAt = player.gameObject.transform;

        if (highlightObj != null)
        {
            Destroy(highlightObj);
        }

        begin = false;
    }

    IEnumerator SwitchTarget()
    {
        canTarget = false;
        yield return new WaitForSeconds(0.5f);
        canTarget = true;
    }

    #region Target

    private void SelectEntities() //Target mode (selection)
    {
        if (PlayerAction == PlayerController.currentAction.isTargeting)
        {
            var cam = Camera.main.GetComponent<CinemachineBrain>().ActiveVirtualCamera;

            //add target to list
            targetList.Add(currentUnit);
            tempList.Remove(currentUnit);

            var length = currentUnit.transform.GetComponentInChildren<SkinnedMeshRenderer>().materials.Length;

            var mats = currentUnit.transform.GetComponentInChildren<SkinnedMeshRenderer>().materials;

            currentUnit.transform.GetComponentInChildren<SkinnedMeshRenderer>().materials = new Material[length + 1];

            var temp = currentUnit.transform.GetComponentInChildren<SkinnedMeshRenderer>().materials;

            temp[0] = highlightMaterial;

            for (int i = 1; i < temp.Length; i++)
            {
                temp[i] = mats[i - 1];
            }

            currentUnit.transform.GetComponentInChildren<SkinnedMeshRenderer>().materials = temp;

            if (focusTarget == tempList.Count)
            {
                focusTarget--;
            }

            if (tempList.Count > 0)
            {
                currentUnit = tempList[focusTarget];

                cam.Follow = currentUnit.gameObject.transform;
                cam.LookAt = currentUnit.gameObject.transform;
            }

            currentNbTarget++;
        }
    }

    private void RemoveEntities() //Target mode (remove/cancel)
    {
        if (PlayerAction == PlayerController.currentAction.isTargeting)
        {
            if (targetList.Count > 0
                && currentNbTarget > 0)
            {
                //remove last target
                Unit oldUnit = targetList.Last();

                tempList.Add(oldUnit);
                targetList.Remove(oldUnit);

                //remove material of the target
                var length = oldUnit.transform.GetComponentInChildren<SkinnedMeshRenderer>().materials.Length;
                var mats = oldUnit.transform.GetComponentInChildren<SkinnedMeshRenderer>().materials;

                List<Material> tempMatList = new List<Material>();

                tempMatList.AddRange(mats);

                tempMatList.Remove(mats[0]);

                oldUnit.transform.GetComponentInChildren<SkinnedMeshRenderer>().materials = tempMatList.ToArray();

                currentNbTarget--;
            }
            else if (currentNbTarget == 0)
            {
                CamOnPlayer();

                PlayerAction = currentAction.isCasting;
                return;
            }
        }
    }

    private void AcceptSelection() //Target mode (accept)
    {
        if (PlayerAction == PlayerController.currentAction.isTargeting
            && targetList.Count > 0)
        {
            //accept
            CamOnPlayer();

            PlayerAction = currentAction.isCasting;
            return;
        }
    }

    private void CancelSelection() //Target mode (cancel)
    {
        if (PlayerAction == PlayerController.currentAction.isTargeting)
        {
            foreach (var target in targetList)
            {
                //remove material of the target
                var length = target.transform.GetComponentInChildren<SkinnedMeshRenderer>().materials.Length;
                var mats = target.transform.GetComponentInChildren<SkinnedMeshRenderer>().materials;

                List<Material> tempMatList = new List<Material>();

                tempMatList.AddRange(mats);

                tempMatList.Remove(mats[0]);

                target.transform.GetComponentInChildren<SkinnedMeshRenderer>().materials = tempMatList.ToArray();
            }

            //cancel
            targetList.Clear();
            CamOnPlayer();

            PlayerAction = currentAction.isCasting;
            return;
        }
    }

    private void SwitchEntity(typeDirection _direction) //Target mode (switch)
    {
        if (PlayerAction == PlayerController.currentAction.isTargeting
            && canTarget)
        {
            var cam = Camera.main.GetComponent<CinemachineBrain>().ActiveVirtualCamera;

            if (_direction == typeDirection.Right)
            {
                if (focusTarget == tempList.Count - 1)
                {
                    focusTarget = 0;
                }
                else if (focusTarget < tempList.Count - 1)
                {
                    focusTarget++;
                }

                currentUnit = tempList[focusTarget];

                cam.Follow = currentUnit.gameObject.transform;
                cam.LookAt = currentUnit.gameObject.transform;
                StartCoroutine(SwitchTarget());

            }
            else if (_direction == typeDirection.Left)
            {
                if (focusTarget == 0)
                {
                    focusTarget = tempList.Count - 1;
                }
                else if (focusTarget > 0)
                {
                    focusTarget--;
                }

                currentUnit = tempList[focusTarget];

                cam.Follow = currentUnit.gameObject.transform;
                cam.LookAt = currentUnit.gameObject.transform;
                StartCoroutine(SwitchTarget());
            }
        }
    }

    private void SelectTarget()
    {
        if (PlayerAction == currentAction.isTargeting)
        {
            //Change Text Display
            if (currentNbTarget == 0)
            {
                FindObjectOfType<HUD>().transform.Find("TargetingMenu").Find("B").GetComponentInChildren<Text>().text= "Cancel";
            }
            else
            {
                FindObjectOfType<HUD>().transform.Find("TargetingMenu").Find("B").GetComponentInChildren<Text>().text = "Remove";
            }

            //display Highlight
            if (highlightObj != null)
            {
                highlightObj.transform.position = new Vector3(
                    currentUnit.transform.position.x,
                    0,
                    currentUnit.transform.position.z);
            }

            if (tempList != null
                && tempList.Count > 0
                && !begin)
            {
                currentUnit = tempList.First();

                highlightObj = Instantiate(highlight);

                var cam = Camera.main.GetComponent<CinemachineBrain>().ActiveVirtualCamera;

                cam.Follow = currentUnit.gameObject.transform;
                cam.LookAt = currentUnit.gameObject.transform;
                begin = !begin;
            }

            if (tempList == null || tempList.Count == 0)
            {
                PlayerAction = currentAction.isCasting;
                CamOnPlayer();
                return;
            }

            if (currentNbTarget == totalNbTarget)
            {
                CamOnPlayer();
                PlayerAction = currentAction.isCasting;
                return;
            }

            if (currentNbTarget < totalNbTarget)
            {
                switch (tempSpell.spellData.Selection)
                {
                    case SpellData.TargetSelection.OneByOne:

                        if (ingameMap.IM.currentInputMap != targetMap)
                        {
                            ingameMap.IM.ChangeInputMap(targetMap);
                        }

                        break;

                    case SpellData.TargetSelection.Random:

                        int j = 0;

                        //select randomly targets 
                        while (j < totalNbTarget)
                        {
                            int rand = Random.Range(1, tempList.Count);

                            Unit tempUnit = tempList[rand];

                            targetList.Add(tempUnit);
                            tempList.Remove(tempUnit);

                            currentNbTarget++;
                            j++;
                        }
                        break;

                    case SpellData.TargetSelection.None:
                        //not possible if there are targets
                        Debug.LogWarning(
                            "Please change your current (spellData.TargetSelection) might be different from None.");
                        break;
                }
            }
        }

        if (PlayerAction == currentAction.isCasting)
        {
            if (targetMap.IM.currentInputMap != ingameMap)
            {
                targetMap.IM.ChangeInputMap(ingameMap);
            }

            if (targetList != null
                && targetList.Count == 0
                && tempSpell.spellData.RangeAction == SpellData.SpellAction.Target)
            {
                PlayerAction = PlayerController.currentAction.isBase;
                ResetTarget();
                CamOnPlayer();
                currentSpell = "";
                return;
            }

            player.ManaCurrent -= tempSpell.spellData.Cost;
            //currentCD = tempSpell.spellData.cooldown;

            //spellsCooldown[nbSpell] = currentCD;

            switch (tempSpell.spellData.animationType)
            {
                case SpellData.AnimationType.Distance:
                    player.BeggingCastAttack();
                    StartCoroutine(ActivateSpell(1.09f));
                    break;
                case SpellData.AnimationType.Melee:
                    player.BeggingCacAttack();
                    StartCoroutine(ActivateSpell(0.5f));
                    break;
            }

            IncantEffectPlayer effectPlayer = GetComponent<IncantEffectPlayer>();

            effectPlayer.StartIncantAnim(tempSpell.spellData.firstElement, tempSpell.spellData.secondElement, 2f);

            var spellGameObject = Toolbox.Instance.GetManager<SpellManager>().GetSpell(currentSpell);

            spellObj = Instantiate(spellGameObject);
            spellObj.GetComponent<Spell>().CurrentUnit = player;
            spellObj.GetComponent<Spell>().targetList = targetList;
            spellObj.transform.position = spawnProjectile.transform.position;
            spellObj.SetActive(false);

            PlayerAction = currentAction.onSpell;

            if (spellObj.GetComponent<Spell>().spellData.RequiredTarget != SpellData.TargetType.Self)
            {
                foreach (var target in targetList)
                {
                    //remove material of the target
                    var length = target.transform.GetComponentInChildren<SkinnedMeshRenderer>().materials.Length;
                    var mats = target.transform.GetComponentInChildren<SkinnedMeshRenderer>().materials;

                    List<Material> tempMatList = new List<Material>();

                    tempMatList.AddRange(mats);

                    tempMatList.Remove(mats[0]);

                    target.transform.GetComponentInChildren<SkinnedMeshRenderer>().materials = tempMatList.ToArray();
                }
            }

            //clear
            spellGameObject = null;
            CamOnPlayer();
            currentSpell = "";
        }
    }

    IEnumerator ActivateSpell(float _timer)
    {
        //1.09f / 0.5f
        yield return new WaitForSeconds(_timer);

        SpellActivation();
    }

    #endregion


    #region Spell
    public void SpellActivation() //Animation Event
    {
        spellObj.SetActive(true);
    }

    public void CreateSpell()
    {
        ResetTarget();

        GameManager gm = Toolbox.Instance.GetManager<GameManager>();

        var spellGameObject = Toolbox.Instance.GetManager<SpellManager>().GetSpell(currentSpell);

        if (spellGameObject == null)
        {
            Debug.LogWarning("wrong spell");
            return;
        }

        tempSpell = spellGameObject.GetComponent<Spell>();

        if (tempSpell == null)
        {
            return;
        }

        if (player.ManaCurrent >= tempSpell.spellData.Cost
            && currentCD <= 0)
        {
            //checkStatus
            List<StatusEffect> tempStatusEffects = player.GetTemporaryEffectList<StatusEffect>();

            int nbStunCount = tempStatusEffects.Count(x => x.typeEffect == SpellData.Effects.Stun);
            int nbParalysisCount = tempStatusEffects.Count(x => x.typeEffect == SpellData.Effects.Paralysis);
            int nbSilenceCount = tempStatusEffects.Count(x => x.typeEffect == SpellData.Effects.Silence);

            if (nbStunCount > 0)
            {
                Toolbox.Instance.GetManager<SoundManager>().Play("Disabled", gameObject);
                return;
            }

            if (nbParalysisCount > 0)
            {
                int rand = Random.Range(1, 100);

                if (rand <= 50)
                {
                    gm.IncrementTurn();
                    Toolbox.Instance.GetManager<SoundManager>().Play("Paralysis", gameObject);
                    return;
                }
            }

            if (nbSilenceCount > 0)
            {
                if (tempSpell.spellData.CategoryType == SpellData.Category.Magical
                    || tempSpell.spellData.CategoryType == SpellData.Category.HealLife
                    || tempSpell.spellData.CategoryType == SpellData.Category.HealMana)
                {
                    Toolbox.Instance.GetManager<SoundManager>().Play("Disabled", gameObject);
                    return;
                }
            }

            SpellData.Category category = tempSpell.spellData.CategoryType;
            SpellData.TargetType targetType = tempSpell.spellData.RequiredTarget;
            SpellData.TargetNumber number = tempSpell.spellData.NumberTarget;
            SpellData.SpellAction action = tempSpell.spellData.RangeAction;
            List<Node> distanceList = new List<Node>();

            distanceList = Toolbox.Instance.GetManager<PathFindingManager>().GetRange(player.GetCurrentNode(), tempSpell.spellData.range);
            totalNbTarget = tempSpell.spellData.nbTargetRequired;

            //check range
            foreach (var node in distanceList)
            {
                if (node.UnitOnThis != null
                    && node.UnitOnThis != player
                    && !tempList.Contains(node.UnitOnThis))
                {
                    tempList.Add(node.UnitOnThis);
                }
            }

            //get target type
            switch (targetType)
            {
                case SpellData.TargetType.Self:
                    break;
                case SpellData.TargetType.Friendly:

                    foreach (var target in tempList)
                    {
                        if (target.typeUnit != Unit.TypeUnit.Ally)
                        {
                            tempList.Remove(target);
                        }
                    }
                    break;
                case SpellData.TargetType.None:
                    break;
                case SpellData.TargetType.Any:
                    break;
                case SpellData.TargetType.Enemy:

                    foreach (var target in tempList)
                    {
                        if (target.typeUnit != Unit.TypeUnit.Enemy)
                        {
                            tempList.Remove(target);
                        }
                    }
                    break;
            }

            //get nbTarget
            switch (action)
            {
                case SpellData.SpellAction.Self:
                    targetList.Add(gm.GetPlayerUnit());
                    PlayerAction = currentAction.isCasting;
                    break;
                case SpellData.SpellAction.Target:
                    //check target type 
                    switch (targetType)
                    {
                        case SpellData.TargetType.Any:

                            if (number == SpellData.TargetNumber.One)
                            {
                                PlayerAction = currentAction.isTargeting;
                            }
                            else if (number == SpellData.TargetNumber.Multiple)
                            {
                                PlayerAction = currentAction.isTargeting;
                            }
                            else if (number == SpellData.TargetNumber.All)
                            {
                                targetList.AddRange(tempList);
                                PlayerAction = currentAction.isCasting;
                            }

                            break;
                        case SpellData.TargetType.Friendly:

                            if (number == SpellData.TargetNumber.One)
                            {
                                PlayerAction = currentAction.isTargeting;
                            }
                            else if (number == SpellData.TargetNumber.Multiple)
                            {
                                PlayerAction = currentAction.isTargeting;
                            }
                            else if (number == SpellData.TargetNumber.All)
                            {
                                targetList.AddRange(tempList);
                                PlayerAction = currentAction.isCasting;
                            }

                            break;
                        case SpellData.TargetType.Enemy:

                            if (number == SpellData.TargetNumber.One)
                            {
                                PlayerAction = currentAction.isTargeting;
                            }
                            else if (number == SpellData.TargetNumber.Multiple)
                            {
                                PlayerAction = currentAction.isTargeting;
                            }
                            else if (number == SpellData.TargetNumber.All)
                            {
                                targetList.AddRange(tempList);
                                PlayerAction = currentAction.isCasting;
                            }

                            break;
                    }

                    break;

                case SpellData.SpellAction.Line:
                    PlayerAction = currentAction.isCasting;
                    break;

                case SpellData.SpellAction.Cone:
                    PlayerAction = currentAction.isCasting;
                    break;

                case SpellData.SpellAction.Near:
                    PlayerAction = currentAction.isCasting;
                    break;

                case SpellData.SpellAction.Area:
                    PlayerAction = currentAction.isCasting;
                    break;

                case SpellData.SpellAction.None:
                    Debug.LogWarning("Please change your current (spellData.TargetType) might be different from None.");
                    currentSpell = "";
                    ResetTarget();
                    spellGameObject = null;
                    break;
            }
        }
        else
        {
            Debug.Log("can't cast this spell, not enough mana or cd");
            currentSpell = "";
            PlayerAction = currentAction.isBase;
            CamOnPlayer();
            ResetTarget();
            spellGameObject = null;
            return;
        }
    }

    #region actionSpells

    //Casting Spell (press buttons)
    private void FirstSpell()
    {
        if (!isMoving && PlayerAction == currentAction.isBase
                      && Toolbox.Instance.GetManager<GameManager>().IsPlayerTurn)
        {
            currentSpell = favoriteSpells[0];
            //currentCD = spellsCooldown[0];

            nbSpell = 0;

            PlayerAction = PlayerController.currentAction.isCreating;
            CreateSpell();
        }
    }

    private void SecondSpell()
    {
        if (!isMoving && PlayerAction == currentAction.isBase
                      && Toolbox.Instance.GetManager<GameManager>().IsPlayerTurn)
        {
            currentSpell = favoriteSpells[1];
            //currentCD = spellsCooldown[1];

            nbSpell = 1;

            PlayerAction = PlayerController.currentAction.isCreating;
            CreateSpell();
        }
    }

    private void ThirdSpell()
    {
        if (!isMoving && PlayerAction == currentAction.isBase
                      && Toolbox.Instance.GetManager<GameManager>().IsPlayerTurn)
        {
            currentSpell = favoriteSpells[2];
            //currentCD = spellsCooldown[2];

            nbSpell = 2;

            PlayerAction = PlayerController.currentAction.isCreating;
            CreateSpell();
        }
    }

    private void LastSpell()
    {
        if (!isMoving && PlayerAction == currentAction.isBase
                      && Toolbox.Instance.GetManager<GameManager>().IsPlayerTurn)
        {
            currentSpell = favoriteSpells[3];
            //currentCD = spellsCooldown[3];

            nbSpell = 3;

            PlayerAction = PlayerController.currentAction.isCreating;
            CreateSpell();
        }
    }
    #endregion

    private void BaseAttack()
    {
        if (!isMoving && PlayerAction == currentAction.isBase
                      && Toolbox.Instance.GetManager<GameManager>().IsPlayerTurn)
        {
            isMoving = true;
            player.BeggingCacAttack();

            spellObj = Instantiate(Toolbox.Instance.GetManager<SpellManager>().GetSpell("BaseAttack"));
            spellObj.GetComponent<Spell>().CurrentUnit = player;
            spellObj.transform.position = spawnProjectile.transform.position;
            spellObj.SetActive(false);

            StartCoroutine(ActivateSpell(0.5f));

            StartCoroutine(EndAction());
        }
    }

    #endregion

    #region Controls

    void SwitchDirection(typeDirection _direction)
    {
        if (!isMoving && PlayerAction == currentAction.isBase
                      && Toolbox.Instance.GetManager<GameManager>().IsPlayerTurn)
        {
            player.thisDirection = _direction;
        }
    }

    void Movements(typeDirection _direction)
    {
        if (!isMoving && PlayerAction == currentAction.isBase
        && Toolbox.Instance.GetManager<GameManager>().IsPlayerTurn)
        {
            isMoving = true;

            if (!player.GetCurrentNode().GetNeighbor((Node.NeighborsEnum)_direction).bIsWall
                && player.GetCurrentNode().GetNeighbor((Node.NeighborsEnum)_direction).UnitOnThis == null)
            {
                player.MoveUnitToDirection(_direction);
                Toolbox.Instance.GetManager<GameManager>().IncrementTurn();
            }
            else
            {
                isMoving = false;
            }
        }
    }

    #endregion

    public void CallEndAction()
    {
        StartCoroutine(EndAction());
    }

    private IEnumerator EndAction()
    {
        isMoving = true;
        PlayerAction = currentAction.endAction;
        yield return new WaitForSeconds(1f);
        PlayerAction = currentAction.isBase;
        isMoving = false;
    }

    void Update()
    {
        if (Toolbox.Instance.GetManager<GameManager>().IsPlayerTurn)
        {
            SelectTarget();
        }
        else
        {
            isMoving = false;
        }
    }
}
