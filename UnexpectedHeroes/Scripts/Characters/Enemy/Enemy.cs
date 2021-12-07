using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Kapy.Move;
using System.Linq;
using Kapy.Statistique;

public class Enemy : Unit
{
    List<Unit> targetList = new List<Unit>();
    List<Unit> tempList = new List<Unit>();
    Spell tempSpell;
    int currentNbTarget = 0;
    int nbspell = 0;
    private GameObject spell;

    public enum EnemyRace
    {
        Fantome,
        Gerbille,
        Golem,
        Serpent
    }

    public enum StateEnemy
    {
        Dead,
        Wandering,
        Aggro,
        CaCAttack,
        CastAttack,
        Escape
    }

    public EnemyRace Race;
    public StateEnemy stateEnemy = StateEnemy.Wandering;

    public int castRange;
    
    Node startPosition;
    [SerializeField] Node targetPosition;

    Unit player;

    Vector2 tempPosPlayer;

    int indexNode = 0;

    #region Spells Stats

    public string[] favoriteSpells = new string[2];
    private readonly int[] spellsCooldown = new int[2];
    private string currentSpell;
    private int currentCD = 0;

    #endregion

    #region Custom
    protected override void CustomStart()
    {
        base.CustomStart();
        // - - - - - - - //

        tempSpell = null;
    }

    protected override void CustomAwake()
    {
        base.CustomAwake();
        // - - - - - - - //
    }

    protected override void CustomUpdate()
    {
        base.CustomUpdate();
        // - - - - - - - //

    }

    protected override void CustomOnDestroy()
    {
        base.CustomOnDestroy();
        // - - - - - - - //

    }
    #endregion

    #region Behaviour Functions
    public override void Action()
    {
        #region Init
        player = gameManager.GetPlayerUnit();

        int zone = 40;

        Vector2Int numRoomPlayer = new Vector2Int(player.PosX / zone, player.PosY / zone);
        Vector2Int numRoomEnemy = new Vector2Int(gameObject.GetComponent<Enemy>().PosX / zone, gameObject.GetComponent<Enemy>().PosY / zone);

        Node currentNode = gameObject.GetComponent<Enemy>().GetCurrentNode();
        #endregion

        #region Gestion StateEnemy

        if (stateUnit == StateUnit.DEATH)
        {
            stateEnemy = StateEnemy.Dead;
        }
        else if (pathFindingManager.GetDistance(currentNode, player.GetCurrentNode()) <= 14)
        {
            IsInCombat = true;
            stateEnemy = StateEnemy.CaCAttack;
        }
        else if (pathFindingManager.GetDistance(currentNode, player.GetCurrentNode()) <= castRange)
        {
            IsInCombat = true;
            stateEnemy = StateEnemy.CastAttack;
        }
        else if (numRoomPlayer == numRoomEnemy)
        {
            IsInCombat = true;
            stateEnemy = StateEnemy.Aggro;
        }
        else if (numRoomPlayer != numRoomEnemy)
        {
            IsInCombat = false;
            stateEnemy = StateEnemy.Wandering;
        }
        #endregion

        #region Behaviour
        switch (stateEnemy)
        {
            case StateEnemy.Dead:

                Debug.Log("STATE - Dead");
                break;

            case StateEnemy.Wandering:

                Debug.Log("STATE - Wandering");
                Wandering(currentNode);

                break;

            case StateEnemy.Aggro:

                Debug.Log("STATE - Aggro");
                Aggro(currentNode);

                break;

            case StateEnemy.CaCAttack:


                Debug.Log("STATE - CaCAttack");
                CaCAttack(player, currentNode);

                break;

            case StateEnemy.CastAttack:


                Debug.Log("STATE - CastAttack");
                CastAttack(player, currentNode);

                break;

            case StateEnemy.Escape:

                Debug.Log("STATE - Escape");

                break;

            default:
                break;
        }
        #endregion

        // - - - - - - - - - - - //
        if (stateEnemy != StateEnemy.CaCAttack && stateEnemy != StateEnemy.CastAttack)
        {
            gameManager.IncrementTurn();
        }
        
    }

    public void Aggro(Node _currentNode)
    {
        startPosition = pathFindingManager.tabNodePath[gameObject.GetComponent<Enemy>().PosX, gameObject.GetComponent<Enemy>().PosY];
        targetPosition = pathFindingManager.tabNodePath[player.PosX, player.PosY];

        if (tempPosPlayer == null || tempPosPlayer != new Vector2(targetPosition.iGridX, targetPosition.iGridY) || path[indexNode].UnitOnThis != null)
        {
            path = pathFindingManager.FindPath(startPosition, targetPosition);
            indexNode = 0;
        }

        CaCOrientation(_currentNode);
        MoveUnitToNode(path[indexNode]);

        indexNode++;

        //Sauvegarde la position du joueur:
        tempPosPlayer = new Vector2(targetPosition.iGridX, targetPosition.iGridY);
    }

    public void Wandering(Node _currentNode)
    {
        indexNode = 0;
        int direction = Random.Range(0, 8);

        switch (direction)
        {
            // Haut :
            case 0:
                path.Add(pathFindingManager.tabNodePath[_currentNode.iGridX, _currentNode.iGridY + 1]);
                break;

            // Haut-Droite :
            case 1:
                path.Add(pathFindingManager.tabNodePath[_currentNode.iGridX + 1, _currentNode.iGridY + 1]);
                break;

            // Droite :
            case 2:
                path.Add(pathFindingManager.tabNodePath[_currentNode.iGridX + 1, _currentNode.iGridY]);
                break;

            // Bas-Droite :
            case 3:
                path.Add(pathFindingManager.tabNodePath[_currentNode.iGridX + 1, _currentNode.iGridY - 1]);
                break;

            // Bas :
            case 4:
                path.Add(pathFindingManager.tabNodePath[_currentNode.iGridX, _currentNode.iGridY - 1]);
                break;

            // Bas-Gauche :
            case 5:
                path.Add(pathFindingManager.tabNodePath[_currentNode.iGridX - 1, _currentNode.iGridY - 1]);
                break;

            // Gauche :
            case 6:
                path.Add(pathFindingManager.tabNodePath[_currentNode.iGridX - 1, _currentNode.iGridY]);
                break;

            // Haut-Gauche :
            case 7:
                path.Add(pathFindingManager.tabNodePath[_currentNode.iGridX - 1, _currentNode.iGridY + 1]);
                break;

            default:
                break;
        }

        CaCOrientation(_currentNode);
        MoveUnitToNode(path[indexNode]);

        path.Clear();
    }
    #endregion

    #region Attack
    public void CaCAttack(Unit _player, Node _currentNode)
    {
        indexNode = 0;

        path.Clear();
        path.Add(_player.GetCurrentNode());
        CaCOrientation(_currentNode);

        switch (Race)
        {
            case EnemyRace.Fantome:

                MeleeSpell();

                break;

            case EnemyRace.Gerbille:

                // ? //

                break;

            case EnemyRace.Golem:

                // ? //

                break;

            case EnemyRace.Serpent:

                // ? //

                break;

            default:
                break;
        }

        path.Clear();
    }

    public void CastAttack(Unit _player, Node _currentNode)
    {
        CastOrientation(_currentNode);

        switch (Race)
        {
            case EnemyRace.Fantome:

                DistanceSpell();

                break;

            case EnemyRace.Gerbille:

                // ? //

                break;

            case EnemyRace.Golem:

                // ? //

                break;

            case EnemyRace.Serpent:

                // ? //

                break;

            default:
                break;
        }


        path.Clear();
    }
    #endregion

    void TargetUnit(int _nbTarget)
    {
        if (tempList == null || tempList.Count == 0)
        {
            CastSpell();
            return;
        }

        if (currentNbTarget == _nbTarget)
        {
            CastSpell();
            return;
        }

        if (currentNbTarget < _nbTarget)
        {
            // Plus tard -> Gérer la prio
            switch (tempSpell.spellData.Selection)
            {
                case SpellData.TargetSelection.OneByOne:

                    targetList.Add(player);
                    CastSpell();
                    break;

                case SpellData.TargetSelection.Random:

                    int i = 0;

                    while (i < _nbTarget)
                    {
                        int rand = Random.Range(1, tempList.Count);

                        Unit tempUnit = tempList[rand];
                        targetList.Add(tempUnit);
                        tempList.Remove(tempUnit);

                        currentNbTarget++;

                        i++;
                    }

                    break;

                case SpellData.TargetSelection.None:
                    break;

                default:
                    break;
            }
        }
    }

    void CastSpell()
    {
        Debug.Log(targetList.Count);
        Debug.Log(tempSpell.spellData.RangeAction);
        Debug.Log(currentSpell);
        

        if (targetList?.Count == 0 && tempSpell.spellData.RangeAction == SpellData.SpellAction.Target)
        {
            currentSpell = "";
            targetList.Clear();
            tempList.Clear();

            return;
        }

        //currentCD = tempSpell.spellData.cooldown;

        //spellsCooldown[nbspell] = currentCD;

        switch (tempSpell.spellData.animationType)
        {
            case SpellData.AnimationType.Distance:

                BeggingCastAttack();
                StartCoroutine(ActivateSpell(0.5f));

                break;

            case SpellData.AnimationType.Melee:

                BeggingCacAttack();
                StartCoroutine(ActivateSpell(0.5f));

                break;

            default:
                break;
        }

        var spellObj = Toolbox.Instance.GetManager<SpellManager>().GetSpell(currentSpell);

        spell = Instantiate(spellObj);

        spell.GetComponent<Spell>().CurrentUnit = gameObject.GetComponent<Enemy>();

        spell.GetComponent<Spell>().targetList = targetList;

        spell.transform.position = gameObject.transform.position; // A changer 

        spell.SetActive(false);

        //Clear

        spellObj = null;
        currentSpell = "";
    }

    IEnumerator ActivateSpell(float _timer)
    {
        yield return new WaitForSeconds(_timer);
        SpellActivation();
    }

    public void SpellActivation()
    {
        spell.SetActive(true);
    }

    void CreateSpell()
    {
        targetList.Clear();
        tempList.Clear();

        var spellObj = Toolbox.Instance.GetManager<SpellManager>().GetSpell(currentSpell);

        if (spellObj == null)
        {
            Debug.Log("ERROR CREATESPELL 1");

            return;
        }

        tempSpell = spellObj.GetComponent<Spell>();

        if (tempSpell == null)
        {
            Debug.Log("ERROR CREATESPELL 2");

            return;
        }

        // Gestion CD & Effect
        if (currentCD <= 0)
        {
            List<StatusEffect> statusEffects = gameObject.GetComponent<Enemy>().GetTemporaryEffectList<StatusEffect>();

            int stun = statusEffects.Count(x => x.typeEffect == SpellData.Effects.Stun);
            int para = statusEffects.Count(x => x.typeEffect == SpellData.Effects.Paralysis);
            int silence = statusEffects.Count(x => x.typeEffect == SpellData.Effects.Silence);

            if (stun > 0)
            {
                return;
            }

            if (para > 0)
            {
                int rand = Random.Range(1, 100);

                if (rand <= 50)
                {
                    Toolbox.Instance.GetManager<GameManager>().IncrementTurn();
                    return;
                }
            }

            if (silence > 0)
            {
                if (tempSpell.spellData.CategoryType == SpellData.Category.Magical || 
                    tempSpell.spellData.CategoryType == SpellData.Category.HealLife ||
                    tempSpell.spellData.CategoryType == SpellData.Category.HealMana)
                {
                    return;
                }
            }

            // Target :
            SpellData.Category category = tempSpell.spellData.CategoryType; 
            SpellData.TargetType targetType = tempSpell.spellData.RequiredTarget; 
            SpellData.TargetNumber number = tempSpell.spellData.NumberTarget; 
            SpellData.SpellAction action = tempSpell.spellData.RangeAction; 
            List<Node> distanceList = new List<Node>();

            distanceList = Toolbox.Instance.GetManager<PathFindingManager>().GetRange(gameObject.GetComponent<Enemy>().GetCurrentNode(), tempSpell.spellData.range);

            int nbTarget = tempSpell.spellData.nbTargetRequired;

            foreach (var node in distanceList)
            {
                if (node?.UnitOnThis != null && node.UnitOnThis != gameObject.GetComponent<Unit>() && !targetList.Contains(node.UnitOnThis))
                {
                    tempList.Add(node.UnitOnThis);
                }
            }

            switch (targetType)
            {
                case SpellData.TargetType.Friendly:

                    foreach (var target in tempList)
                    {
                        if (target.typeUnit != Unit.TypeUnit.Enemy)
                        {
                            tempList.Remove(target);
                        }
                    }

                    break;

                case SpellData.TargetType.Enemy:

                    foreach (var target in tempList)
                    {
                        if (target.typeUnit == Unit.TypeUnit.Enemy)
                        {
                            tempList.Remove(target);
                        }
                    }

                    break;

                case SpellData.TargetType.Any:
                    break;

                case SpellData.TargetType.Self:
                    break;

                case SpellData.TargetType.None:
                    break;

                default:
                    break;
            }

            switch (action)
            {
                case SpellData.SpellAction.Line:

                    CastSpell();

                    break;

                case SpellData.SpellAction.Cone:

                    CastSpell();

                    break;

                case SpellData.SpellAction.Near:

                    CastSpell();

                    break;

                case SpellData.SpellAction.Area:

                    CastSpell();

                    break;

                case SpellData.SpellAction.Self:

                    targetList.Add(gameObject.GetComponent<Unit>());
                    CastSpell();

                    break;

                case SpellData.SpellAction.Target:

                    switch (targetType)
                    {
                        case SpellData.TargetType.Friendly:

                            if (number == SpellData.TargetNumber.One)
                            {
                                TargetUnit(nbTarget);
                            }
                            else if (number == SpellData.TargetNumber.Multiple)
                            {
                                TargetUnit(nbTarget);
                            }
                            else if (number == SpellData.TargetNumber.All)
                            {
                                targetList.AddRange(tempList);
                                CastSpell();
                            }

                            break;
                        case SpellData.TargetType.Enemy:

                            if (number == SpellData.TargetNumber.One)
                            {
                                TargetUnit(nbTarget);
                            }
                            else if (number == SpellData.TargetNumber.Multiple)
                            {
                                TargetUnit(nbTarget);
                            }
                            else if (number == SpellData.TargetNumber.All)
                            {
                                targetList.AddRange(tempList);
                                CastSpell();
                            }

                            break;
                        case SpellData.TargetType.Any:

                            if (number == SpellData.TargetNumber.One)
                            {
                                TargetUnit(nbTarget);
                            }
                            else if(number == SpellData.TargetNumber.Multiple)
                            {
                                TargetUnit(nbTarget);
                            }
                            else if(number == SpellData.TargetNumber.All)
                            {
                                targetList.AddRange(tempList);
                                CastSpell();
                            }

                            break;
                        case SpellData.TargetType.Self:
                            break;
                        case SpellData.TargetType.None:
                            break;
                        default:
                            break;
                    }

                    break;

                case SpellData.SpellAction.None:

                    Debug.LogWarning("Wrong Target");
                    currentSpell = "";
                    spellObj = null;
                    targetList.Clear();
                    tempList.Clear();

                    break;

                default:
                    break;
            }
        }

        else
        {
            Debug.LogError("Cant Cast Spell");
            currentSpell = "";
            spellObj = null;
            targetList.Clear();
            tempList.Clear();
            return;
        }
    }

    void MeleeSpell()
    {
        currentSpell = favoriteSpells[0];
        //currentCD = spellsCooldown[0];
        nbspell = 0;

        CreateSpell();

    }

    void DistanceSpell()
    {
        currentSpell = favoriteSpells[1];
        //currentCD = spellsCooldown[1];
        nbspell = 1;

        CreateSpell();
    }

    #region Orientation
    public void CaCOrientation(Node _currentNode)
    {
        if (path[indexNode] == _currentNode.GetNeighbor(Node.NeighborsEnum.Up))
        {
            thisDirection = Kapy.Move.typeDirection.Up;
        }
        else if (path[indexNode] == _currentNode.GetNeighbor(Node.NeighborsEnum.UpRight))
        {
            thisDirection = Kapy.Move.typeDirection.UpRight;
        }
        else if (path[indexNode] == _currentNode.GetNeighbor(Node.NeighborsEnum.Right))
        {
            thisDirection = Kapy.Move.typeDirection.Right;
        }
        else if (path[indexNode] == _currentNode.GetNeighbor(Node.NeighborsEnum.DownRight))
        {
            thisDirection = Kapy.Move.typeDirection.DownRight;
        }
        else if (path[indexNode] == _currentNode.GetNeighbor(Node.NeighborsEnum.Down))
        {
            thisDirection = Kapy.Move.typeDirection.Down;
        }
        else if (path[indexNode] == _currentNode.GetNeighbor(Node.NeighborsEnum.DownLeft))
        {
            thisDirection = Kapy.Move.typeDirection.DownLeft;
        }
        else if (path[indexNode] == _currentNode.GetNeighbor(Node.NeighborsEnum.Left))
        {
            thisDirection = Kapy.Move.typeDirection.Left;
        }
        else if (path[indexNode] == _currentNode.GetNeighbor(Node.NeighborsEnum.Upleft))
        {
            thisDirection = Kapy.Move.typeDirection.UpLeft;
        }
    }

    public void CastOrientation(Node _currentNode)
    {
        // V2.0 :

        //Angle :
        // 0 / 45 / 90 / 135 / 180 / 225 / 270 / 315

        // 0 - 22.5 - 45
        // 45 - 67.5 - 90
        // 90 - 112.5 - 135
        // 135 - 157.5 - 180
        

        Vector3 enemy2Player = new Vector3(player.PosX - gameObject.GetComponent<Enemy>().PosX, 0, player.PosY - gameObject.GetComponent<Enemy>().PosY);
        Vector3 upRightEnemy = new Vector3(1, 0, 1);
        float angle = Vector3.SignedAngle(upRightEnemy, enemy2Player, Vector3.up);

        // Both :
        if (angle >= -22.5f && angle <= 22.5f)
        {
            thisDirection = typeDirection.UpRight;
        }
        else if ((angle >= 157.5 && angle <= 180f) || (angle <= -157.5 && angle >= -180f))
        {
            thisDirection = typeDirection.DownLeft;
        }

        //Positive angle :
        else if(angle >= 22.5f && angle <= 67.5f)
        {
            thisDirection = typeDirection.Right;
        }
        else if (angle >= 67.5f && angle <= 112.5f)
        {
            thisDirection = typeDirection.DownRight;
        }
        else if (angle >= 112.5f && angle <= 157.5f)
        {
            thisDirection = typeDirection.Down;
        }

        //Negative angle :
        else if (angle <= -22.5f && angle >= -67.5f)
        {
            thisDirection = typeDirection.Up;
        }
        else if (angle <= -67.5f && angle >= -112.5f)
        {
            thisDirection = typeDirection.UpLeft;
        }
        else if (angle <= -112.5f && angle >= -157.5f)
        {
            thisDirection = typeDirection.Left;
        }

    }
    #endregion
}

