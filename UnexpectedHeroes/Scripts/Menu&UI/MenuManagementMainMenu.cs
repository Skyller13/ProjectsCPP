using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MenuManagementMainMenu : MenuManagement
{
    SoundManager soundManager;

    public override void CustomAwake()
    {
        base.CustomAwake();
    }

    private void Start()
    {
        soundManager = Toolbox.Instance.GetManager<SoundManager>();
    }

    // Update is called once per frame
    public override void CustomUpdate()
    {
        base.CustomUpdate();

        Button[] listMenu = GetComponentsInChildren<Button>();
        foreach (var item in listMenu)
        {
            if (item.transform.position == Input.mousePosition)
            {
                soundManager.Play("MenuSelection");
            }
        }
    }

    public override void ExitMenu()
    {
        if (menuInGame.ingameStateMachine.selectedTypeState != MenuIngameStateMachine.stateMenuIngame.GLOBAL)
        {
            return;
        }

        base.ExitMenu();
    }
}
