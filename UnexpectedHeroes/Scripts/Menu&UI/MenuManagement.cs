using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MenuManagement : MonoBehaviour
{
    public MenuType Type;

    [SerializeField] Button ExitButton;

    MenuIngameStateMachine ingameStateMachine;

    [SerializeField] protected MenuInGame menuInGame;

    // Start is called before the first frame update
    void Awake()
    {
        CustomAwake();
    }

    virtual public void CustomAwake()
    {
        ingameStateMachine = FindObjectOfType<MenuIngameStateMachine>();

        menuInGame = FindObjectOfType<MenuInGame>();

        Button[] buttons = GetComponentsInChildren<Button>();
        foreach (var item in buttons)
        {
            if (item.name == "ExitButton")
            {
                ExitButton = item;
                break;
            }
        }

        ExitButton.onClick.AddListener(ExitMenu);
    }

    private void Update()
    {
        CustomUpdate();
    }

    private void OnEnable()
    {
        menuInGame.IGMMap.ONMenuQuit += ExitMenu;
    }

    private void OnDisable()
    {
        menuInGame.IGMMap.ONMenuQuit -= ExitMenu;
    }

    virtual public void ExitMenu()
    {
        gameObject.SetActive(false);

        menuInGame.ExitMenu();
    }

    virtual public void CustomUpdate()
    {
        
    }
}
