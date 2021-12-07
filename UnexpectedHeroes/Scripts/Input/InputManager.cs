using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Kapy.Move;
using System;

public delegate void DirectionDelegate(typeDirection _dir);
public class InputManager : MonoBehaviour
{
    public enum typeInputMap
    {
        INGAME,
        WHEEL,
        INGAME_MENU,
        MENU
    }

    public enum typeControllerInput
    {
        STICK_LEFT,
        STICK_RIGHT,
        CROSS,
        TRIGGER_LEFT,
        TRIGGER_RIGHT,
        BUTTON_A,
        BUTTON_B,
        BUTTON_X,
        BUTTON_Y,
        BUMPER_RB,
        BUMPER_LB,
        BUTTON_START,
        BUTTON_BACK,
        BUTTON_STICK_LEFT,
        BUTTON_STICK_RIGHT
    }


    public Dictionary<Type, KapyInputMap> inputMaps = new Dictionary<Type, KapyInputMap>();
    public KapyInputMap currentInputMap = null;
    [SerializeField] InputManagerConfig config;

    GameManager GM;
    bool IsInit = false;

    #region Custom Kapy Input Inspector

    #region Source functions
    [System.Serializable]
    public abstract class KapyControllerInputInspector
    {
        public string accessCode = "";
        public bool isUsed = false;
        protected bool isPressLocked = false;
        protected bool isHoldLocked = false;
        protected bool isEvent = false;
        public float pressedTimer = 0f;
        public float lastPressedTime = 0f;
        float HoldTimer = 0f;

        //Input Config
        [HideInInspector] public float TapMin = 0.05f;
        [HideInInspector] public float TapMax = 0.5f;
        [HideInInspector] public float PressMin = 0.5f;
        [HideInInspector] public float ReleaseMin = 0.5f;
        [HideInInspector] public float HoldMin = 1f;

        public typeControllerInput typeButton = typeControllerInput.BUTTON_A;

        [HideInInspector] public InputManager refInputManager = null;

        public void Configure(InputManagerConfig.ButtonInputConfig _config)
        {
            TapMin = _config.TapMinDuration;
            TapMax = _config.TapMaxDuration;
            PressMin = _config.PressMinDuration;
            ReleaseMin = _config.ReleaseMinDuration;
            HoldMin = _config.HoldMinDuration;
        }

        public virtual void Update()
        {
            refInputManager.SetBoolCheck(typeButton, isUsed);

            if (isUsed && pressedTimer > PressMin && !isPressLocked)
            {
                this.CallPress();
                isPressLocked = true;
            }
            else if (isUsed && pressedTimer > HoldMin && !isHoldLocked)
            {
                this.CallHold();
                isHoldLocked = true;
            }
            if (!isEvent && isUsed)
            {
                isEvent = true;                
            }
            if (isEvent && !isUsed)
            {
                if (lastPressedTime >= TapMin && lastPressedTime < TapMax)
                {
                    this.CallTap();

                }
                else if (lastPressedTime >= ReleaseMin)
                {
                    this.CallRelease();
                }
                isEvent = false;
                isPressLocked = false;
                isHoldLocked = false;
            }

            if (isHoldLocked)
            {
                HoldTimer += Time.unscaledDeltaTime;
                if (HoldTimer> HoldMin)
                {
                    isHoldLocked = false;
                    HoldTimer = 0f;
                }
            }
           
        }

        

        public virtual void CallTap()
        {
            refInputManager.CallTap(typeButton);
        }

        public virtual void CallPress()
        {
            refInputManager.CallPress(typeButton); 
        }

        public virtual void CallRelease()
        {
            refInputManager.CallRelease(typeButton);
        }

        public virtual void CallHold()
        {
            refInputManager.CallHold(typeButton);
        }
    }



    [System.Serializable]
    public class KapyJoyStickInpector : KapyControllerInputInspector
    {
        public Vector2 axis = Vector2.zero;
        public float angle = 0f;
        public typeDirection currentDirection = typeDirection.None;
        string accessCodeX = "";
        string accessCodeY = "";

        public override void Update()
        {
            
            if (accessCodeX == "")
            {
                accessCodeX = accessCode + "X";
            }
            if (accessCodeY == "")
            {
                accessCodeY = accessCode + "Y";
            }


            axis.x = Input.GetAxis(accessCodeX);
            axis.y = Input.GetAxis(accessCodeY);
            axis.Normalize();
            angle = (Vector2.SignedAngle(axis, Vector2.up) + 360 /*+ 90 * (position - 1)*/) % 360;


            if (Mathf.Abs(axis.magnitude) > 0.1f)
            {
                isUsed = true;
                pressedTimer += Time.unscaledDeltaTime;
                lastPressedTime = pressedTimer;
                currentDirection = (typeDirection)(Convert.ToInt32(angle / (360f / 8f)) % 8);
            }
            else
            {
                isUsed = false;
               
                pressedTimer = 0f;
            }


            base.Update();
        }

        

        public override void CallTap()
        {
           refInputManager.CallTap(typeButton, currentDirection);
        }

        public override void CallPress()
        {
            if (Mathf.Abs(axis.magnitude) > 0.5f)
                refInputManager.CallPress(typeButton, currentDirection);
        }

        public override void CallRelease()
        {
            refInputManager.CallRelease(typeButton, currentDirection);
        }

        public override void CallHold()
        {
            if (Mathf.Abs(axis.magnitude) > 0.5f)
                refInputManager.CallHold(typeButton, currentDirection);
        }

    }

    [System.Serializable]
    public class KapyTriggerInspector : KapyControllerInputInspector
    {
        public float axis = 0f;
        bool isRight = true;

        public void SetIfRightOrLeft(bool _b)
        {
            isRight = _b;
        }

        public override void Update()
        {
            if (isRight)
            {
                float temp = Input.GetAxisRaw(accessCode);
                if (temp >= 0f)
                {
                    axis = Mathf.Abs(temp);
                }
                else
                {
                    axis = 0f;
                }
            }
            else
            {
                float temp = Input.GetAxisRaw(accessCode);
                if (temp < 0f)
                {
                    axis = Mathf.Abs(temp);
                }
                else
                {
                    axis = 0f;
                }
            }

            if (axis > 0.1f)
            {
                isUsed = true;
                pressedTimer += Time.unscaledDeltaTime;
            }
            else
            {
                isUsed = false;
                lastPressedTime = pressedTimer;
                pressedTimer = 0f;
            }



            base.Update();
        }

        public override void CallTap()
        {
                refInputManager.CallTap(typeButton);
        }

        public override void CallPress()
        {
            if (axis > 0.5f)
                refInputManager.CallPress(typeButton);
        }

        public override void CallRelease()
        {
                refInputManager.CallRelease(typeButton);
        }

        public override void CallHold()
        {
            if (axis > 0.5f)
                refInputManager.CallHold(typeButton);
        }
    }

    [System.Serializable]
    public class KapyButtonInspector : KapyControllerInputInspector
    {
        public override void Update()
        {
            isUsed = Input.GetButton(accessCode);

            if (isUsed)
            {
                pressedTimer += Time.unscaledDeltaTime;
                lastPressedTime = pressedTimer;
            }
            else
            {
                pressedTimer = 0f;
            }


            base.Update();
        }
    }

    [System.Serializable]
    public class KapyKeyInspector : KapyControllerInputInspector
    {
        public override void Update()
        {
            isUsed = Input.GetKey(accessCode);

            if (isUsed)
            {
                pressedTimer += Time.unscaledDeltaTime;
                lastPressedTime = pressedTimer;
            }
            else
            {
                
                pressedTimer = 0f;
            }


            base.Update();
        }
    }

    [System.Serializable]
    public class ButtonBoolCheck
    {
        public bool StickLeft = false;
        public bool StickRight = false;
        public bool Cross = false;
        public bool TriggerLeft = false;
        public bool TriggerRight = false;
        public bool ButtonA = false;
        public bool ButtonB = false;
        public bool ButtonX = false;
        public bool ButtonY = false;
        public bool BumperLB = false;
        public bool BumperRB = false;
        public bool ButtonStart = false;
        public bool ButtonBack = false;
        public bool ButtonStickLeft = false;
        public bool ButtonStickRight = false;
    }
    public ButtonBoolCheck buttonBoolCheck = new ButtonBoolCheck();

    public void SetBoolCheck(typeControllerInput _type, bool _isUsed) //TODO : put this function inside each button inspector ^^
    {
        switch (_type)
        {
            case typeControllerInput.STICK_LEFT:
                buttonBoolCheck.StickLeft = _isUsed;
                break;
            case typeControllerInput.STICK_RIGHT:
                buttonBoolCheck.StickRight = _isUsed;
                break;
            case typeControllerInput.CROSS:
                buttonBoolCheck.Cross = _isUsed;
                break;
            case typeControllerInput.TRIGGER_LEFT:
                buttonBoolCheck.TriggerLeft = _isUsed;
                break;
            case typeControllerInput.TRIGGER_RIGHT:
                buttonBoolCheck.TriggerRight = _isUsed;
                break;
            case typeControllerInput.BUTTON_A:
                buttonBoolCheck.ButtonA = _isUsed;
                break;
            case typeControllerInput.BUTTON_B:
                buttonBoolCheck.ButtonB = _isUsed;
                break;
            case typeControllerInput.BUTTON_X:
                buttonBoolCheck.ButtonX = _isUsed;
                break;
            case typeControllerInput.BUTTON_Y:
                buttonBoolCheck.ButtonY = _isUsed;
                break;
            case typeControllerInput.BUMPER_RB:
                buttonBoolCheck.BumperRB = _isUsed;
                break;
            case typeControllerInput.BUMPER_LB:
                buttonBoolCheck.BumperLB = _isUsed;
                break;
            case typeControllerInput.BUTTON_START:
                buttonBoolCheck.ButtonStart = _isUsed;
                break;
            case typeControllerInput.BUTTON_BACK:
                buttonBoolCheck.ButtonBack = _isUsed;
                break;
            case typeControllerInput.BUTTON_STICK_LEFT:
                buttonBoolCheck.ButtonStickLeft = _isUsed;
                break;
            case typeControllerInput.BUTTON_STICK_RIGHT:
                buttonBoolCheck.ButtonStickRight = _isUsed;
                break;
            default:
                break;
        }
    }
    #endregion

    #region XBOX Controller
    [System.Serializable]
    public class KapyXBoxControllerInspector
    {
        public KapyJoyStickInpector stickLeft = new KapyJoyStickInpector();
        public KapyJoyStickInpector stickRight = new KapyJoyStickInpector();
        public KapyJoyStickInpector Cross = new KapyJoyStickInpector();
        public KapyTriggerInspector triggerLeft = new KapyTriggerInspector();
        public KapyTriggerInspector triggerRight = new KapyTriggerInspector();
        public KapyButtonInspector buttonA = new KapyButtonInspector();
        public KapyButtonInspector buttonB = new KapyButtonInspector();
        public KapyButtonInspector buttonX = new KapyButtonInspector();
        public KapyButtonInspector buttonY = new KapyButtonInspector();
        public KapyButtonInspector bumperRB = new KapyButtonInspector();
        public KapyButtonInspector bumperLB = new KapyButtonInspector();
        public KapyButtonInspector buttonStart = new KapyButtonInspector();
        public KapyButtonInspector buttonBack = new KapyButtonInspector();
        public KapyButtonInspector stickLeftButton = new KapyButtonInspector();
        public KapyButtonInspector stickRightButton = new KapyButtonInspector();

        public KapyJoyStickInpector stickLeftKeyboard = new KapyJoyStickInpector();
        public KapyJoyStickInpector stickRightKeyboard = new KapyJoyStickInpector();
        public KapyJoyStickInpector CrossKeyboard = new KapyJoyStickInpector();
        public KapyTriggerInspector triggerLeftKeyboard = new KapyTriggerInspector();
        public KapyTriggerInspector triggerRightKeyboard = new KapyTriggerInspector();

        public void Init()
        {
            InputManager IM = Toolbox.Instance.GetManager<InputManager>();
            stickLeft.accessCode = "KapyStickLeft";
            stickLeft.typeButton = typeControllerInput.STICK_LEFT;
            stickLeft.refInputManager = IM;
            stickLeft.Configure(IM.config.StickLeft);
            stickRight.accessCode = "KapyStickRight";
            stickRight.typeButton = typeControllerInput.STICK_RIGHT;
            stickRight.refInputManager = IM;
            stickRight.Configure(IM.config.StickRight);
            Cross.accessCode = "KapyCross";
            Cross.typeButton = typeControllerInput.CROSS;
            Cross.refInputManager = IM;
            Cross.Configure(IM.config.Cross);
            triggerLeft.accessCode = "KapyTriggers";
            triggerLeft.typeButton = typeControllerInput.TRIGGER_LEFT;
            triggerLeft.refInputManager = IM;
            triggerLeft.Configure(IM.config.Trigger);
            triggerLeft.SetIfRightOrLeft(false);
            triggerRight.accessCode = "KapyTriggers";
            triggerRight.typeButton = typeControllerInput.TRIGGER_RIGHT;
            triggerRight.refInputManager = IM;
            triggerRight.Configure(IM.config.Trigger);
            triggerRight.SetIfRightOrLeft(true);
            buttonA.accessCode = "KapyButtonA";
            buttonA.typeButton = typeControllerInput.BUTTON_A;
            buttonA.refInputManager = IM;
            buttonA.Configure(IM.config.ButtonA);
            buttonB.accessCode = "KapyButtonB";
            buttonB.typeButton = typeControllerInput.BUTTON_B;
            buttonB.refInputManager = IM;
            buttonB.Configure(IM.config.ButtonB);
            buttonX.accessCode = "KapyButtonX";
            buttonX.typeButton = typeControllerInput.BUTTON_X;
            buttonX.refInputManager = IM;
            buttonX.Configure(IM.config.ButtonX);
            buttonY.accessCode = "KapyButtonY";
            buttonY.typeButton = typeControllerInput.BUTTON_Y;
            buttonY.refInputManager = IM;
            buttonY.Configure(IM.config.ButtonY);
            bumperLB.accessCode = "KapyBumperLB";
            bumperLB.typeButton = typeControllerInput.BUMPER_LB;
            bumperLB.refInputManager = IM;
            bumperLB.Configure(IM.config.BumperLB);
            bumperRB.accessCode = "KapyBumperRB";
            bumperRB.typeButton = typeControllerInput.BUMPER_RB;
            bumperRB.refInputManager = IM;
            bumperRB.Configure(IM.config.BumperRB);
            buttonStart.accessCode = "KapyButtonStart";
            buttonStart.typeButton = typeControllerInput.BUTTON_START;
            buttonStart.refInputManager = IM;
            buttonStart.Configure(IM.config.ButtonStart);
            buttonBack.accessCode = "KapyButtonBack";
            buttonBack.typeButton = typeControllerInput.BUTTON_BACK;
            buttonBack.refInputManager = IM;
            buttonBack.Configure(IM.config.ButtonBack);
            stickLeftButton.accessCode = "KapyStickLeftButton";
            stickLeftButton.typeButton = typeControllerInput.BUTTON_STICK_LEFT;
            stickLeftButton.refInputManager = IM;
            stickLeftButton.Configure(IM.config.ButtonStickLeft);
            stickRightButton.accessCode = "KapyStickRightButton";
            stickRightButton.typeButton = typeControllerInput.BUTTON_STICK_RIGHT;
            stickRightButton.refInputManager = IM;
            stickRightButton.Configure(IM.config.ButtonStickRight);

            stickLeftKeyboard.accessCode = "KapyStickLeftKeyboard";
            stickLeftKeyboard.typeButton = typeControllerInput.STICK_LEFT;
            stickLeftKeyboard.refInputManager = IM;
            stickLeftKeyboard.Configure(IM.config.StickLeftKeyboard);
            stickRightKeyboard.accessCode = "KapyStickRightKeyboard";
            stickRightKeyboard.typeButton = typeControllerInput.STICK_RIGHT;
            stickRightKeyboard.refInputManager = IM;
            stickRightKeyboard.Configure(IM.config.StickRightKeyboard);
            CrossKeyboard.accessCode = "KapyCrossKeyboard";
            CrossKeyboard.typeButton = typeControllerInput.CROSS;
            CrossKeyboard.refInputManager = IM;
            CrossKeyboard.Configure(IM.config.CrossKeyboard);
            triggerLeftKeyboard.accessCode = "KapyTriggerKeyboard";
            triggerLeftKeyboard.typeButton = typeControllerInput.TRIGGER_LEFT;
            triggerLeftKeyboard.refInputManager = IM;
            triggerLeftKeyboard.Configure(IM.config.TriggerKeyboard);
            triggerLeftKeyboard.SetIfRightOrLeft(false);
            triggerRightKeyboard.accessCode = "KapyTriggerKeyboard";
            triggerRightKeyboard.typeButton = typeControllerInput.TRIGGER_RIGHT;
            triggerRightKeyboard.refInputManager = IM;
            triggerRightKeyboard.Configure(IM.config.TriggerKeyboard);
            triggerRightKeyboard.SetIfRightOrLeft(true);
        }

        public void Update()
        {
            stickLeft.Update();
            
            stickRight.Update();
            Cross.Update();
            triggerLeft.Update();
            triggerRight.Update();
            buttonA.Update();
            buttonB.Update();
            buttonX.Update();
            buttonY.Update();
            bumperRB.Update();
            bumperLB.Update();
            buttonStart.Update();
            buttonBack.Update();
            stickLeftButton.Update();
            stickRightButton.Update();

            stickLeftKeyboard.Update();
            stickRightKeyboard.Update();
            CrossKeyboard.Update();
            triggerLeftKeyboard.Update();
            triggerRightKeyboard.Update();
        }
    }
    #endregion

   

    #endregion

    public KapyXBoxControllerInspector XBoxControllerInspector = new KapyXBoxControllerInspector();
    


    #region Event call functions
    public event DirectionDelegate ONStickLeftTap;
    public event DirectionDelegate ONStickLeftPress;
    public event DirectionDelegate ONStickLeftRelease;
    public event DirectionDelegate ONStickLeftHold;

    public event DirectionDelegate ONStickRightTap;
    public event DirectionDelegate ONStickRightPress;
    public event DirectionDelegate ONStickRightRelease;
    public event DirectionDelegate ONStickRightHold;

    public event DirectionDelegate ONCrossTap;
    public event DirectionDelegate ONCrossPress;
    public event DirectionDelegate ONCrossRelease;
    public event DirectionDelegate ONCrossHold;

    public event VoidDelegate ONTriggerLeftTap;
    public event VoidDelegate ONTriggerLeftPress;
    public event VoidDelegate ONTriggerLeftRelease;
    public event VoidDelegate ONTriggerLeftHold;

    public event VoidDelegate ONTriggerRightTap;
    public event VoidDelegate ONTriggerRightPress;
    public event VoidDelegate ONTriggerRightRelease;
    public event VoidDelegate ONTriggerRightHold;

    public event VoidDelegate ONButtonATap;
    public event VoidDelegate ONButtonAPress;
    public event VoidDelegate ONButtonARelease;
    public event VoidDelegate ONButtonAHold;

    public event VoidDelegate ONButtonBTap;
    public event VoidDelegate ONButtonBPress;
    public event VoidDelegate ONButtonBRelease;
    public event VoidDelegate ONButtonBHold;

    public event VoidDelegate ONButtonXTap;
    public event VoidDelegate ONButtonXPress;
    public event VoidDelegate ONButtonXRelease;
    public event VoidDelegate ONButtonXHold;

    public event VoidDelegate ONButtonYTap;
    public event VoidDelegate ONButtonYPress;
    public event VoidDelegate ONButtonYRelease;
    public event VoidDelegate ONButtonYHold;

    public event VoidDelegate ONBumperRBTap;
    public event VoidDelegate ONBumperRBPress;
    public event VoidDelegate ONBumperRBRelease;
    public event VoidDelegate ONBumperRBHold;

    public event VoidDelegate ONBumperLBTap;
    public event VoidDelegate ONBumperLBPress;
    public event VoidDelegate ONBumperLBRelease;
    public event VoidDelegate ONBumperLBHold;

    public event VoidDelegate ONButtonStartTap;
    public event VoidDelegate ONButtonStartPress;
    public event VoidDelegate ONButtonStartRelease;
    public event VoidDelegate ONButtonStartHold;

    public event VoidDelegate ONButtonBackTap;
    public event VoidDelegate ONButtonBackPress;
    public event VoidDelegate ONButtonBackRelease;
    public event VoidDelegate ONButtonBackHold;

    public event VoidDelegate ONButtonStickLeftTap;
    public event VoidDelegate ONButtonStickLeftPress;
    public event VoidDelegate ONButtonStickLeftRelease;
    public event VoidDelegate ONButtonStickLeftHold;

    public event VoidDelegate ONButtonStickRightTap;
    public event VoidDelegate ONButtonStickRightPress;
    public event VoidDelegate ONButtonStickRightRelease;
    public event VoidDelegate ONButtonStickRightHold;

    protected void CallTap(typeControllerInput _typeControllerInput, typeDirection _typeDirection = typeDirection.None)
    {
        //Debug.Log("tap");
        switch (_typeControllerInput)
        {
            case typeControllerInput.STICK_LEFT:
                ONStickLeftTap?.Invoke(_typeDirection);
                break;
            case typeControllerInput.STICK_RIGHT:
                ONStickRightTap?.Invoke(_typeDirection);
                break;
            case typeControllerInput.CROSS:
                ONCrossTap?.Invoke(_typeDirection);
                break;
            case typeControllerInput.TRIGGER_LEFT:
                ONTriggerLeftTap?.Invoke();
                break;
            case typeControllerInput.TRIGGER_RIGHT:
                ONTriggerRightTap?.Invoke();
                break;
            case typeControllerInput.BUTTON_A:
                ONButtonATap?.Invoke();
                break;
            case typeControllerInput.BUTTON_B:
                ONButtonBTap?.Invoke();
                break;
            case typeControllerInput.BUTTON_X:
                ONButtonXTap?.Invoke();
                break;
            case typeControllerInput.BUTTON_Y:
                ONButtonYTap?.Invoke();
                break;
            case typeControllerInput.BUMPER_RB:
                ONBumperRBTap?.Invoke();
                break;
            case typeControllerInput.BUMPER_LB:
                ONBumperLBTap?.Invoke();
                break;
            case typeControllerInput.BUTTON_START:
                ONButtonStartTap?.Invoke();
                break;
            case typeControllerInput.BUTTON_BACK:
                ONButtonBackTap?.Invoke();
                break;
            case typeControllerInput.BUTTON_STICK_LEFT:
                ONButtonStickLeftTap?.Invoke();
                break;
            case typeControllerInput.BUTTON_STICK_RIGHT:
                ONButtonStickRightTap?.Invoke();
                break;
            default:
                break;
        }
    }

    protected void CallPress(typeControllerInput _typeControllerInput, typeDirection _typeDirection = typeDirection.None)
    {
        //Debug.Log("press");
        switch (_typeControllerInput)
        {
            case typeControllerInput.STICK_LEFT:
                ONStickLeftPress?.Invoke(_typeDirection);
                break;
            case typeControllerInput.STICK_RIGHT:
                ONStickRightPress?.Invoke(_typeDirection);
                break;
            case typeControllerInput.CROSS:
                ONCrossPress?.Invoke(_typeDirection);
                break;
            case typeControllerInput.TRIGGER_LEFT:
                ONTriggerLeftPress?.Invoke();
                break;
            case typeControllerInput.TRIGGER_RIGHT:
                ONTriggerRightPress?.Invoke();
                break;
            case typeControllerInput.BUTTON_A:
                ONButtonAPress?.Invoke();
                break;
            case typeControllerInput.BUTTON_B:
                ONButtonBPress?.Invoke();
                break;
            case typeControllerInput.BUTTON_X:
                ONButtonXPress?.Invoke();
                break;
            case typeControllerInput.BUTTON_Y:
                ONButtonYPress?.Invoke();
                break;
            case typeControllerInput.BUMPER_RB:
                ONBumperRBPress?.Invoke();
                break;
            case typeControllerInput.BUMPER_LB:
                ONBumperLBPress?.Invoke();
                break;
            case typeControllerInput.BUTTON_START:
                ONButtonStartPress?.Invoke();
                break;
            case typeControllerInput.BUTTON_BACK:
                ONButtonBackPress?.Invoke();
                break;
            case typeControllerInput.BUTTON_STICK_LEFT:
                ONButtonStickLeftPress?.Invoke();
                break;
            case typeControllerInput.BUTTON_STICK_RIGHT:
                ONButtonStickRightPress?.Invoke();
                break;
            default:
                break;
        }
    }

    protected void CallRelease(typeControllerInput _typeControllerInput, typeDirection _typeDirection = typeDirection.None)
    {
        //Debug.Log("Release");
        switch (_typeControllerInput)
        {
            case typeControllerInput.STICK_LEFT:
                ONStickLeftRelease?.Invoke(_typeDirection);
                break;
            case typeControllerInput.STICK_RIGHT:
                ONStickRightRelease?.Invoke(_typeDirection);
                break;
            case typeControllerInput.CROSS:
                ONCrossRelease?.Invoke(_typeDirection);
                break;
            case typeControllerInput.TRIGGER_LEFT:
                ONTriggerLeftRelease?.Invoke();
                break;
            case typeControllerInput.TRIGGER_RIGHT:
                ONTriggerRightRelease?.Invoke();
                break;
            case typeControllerInput.BUTTON_A:
                ONButtonARelease?.Invoke();
                break;
            case typeControllerInput.BUTTON_B:
                ONButtonBRelease?.Invoke();
                break;
            case typeControllerInput.BUTTON_X:
                ONButtonXRelease?.Invoke();
                break;
            case typeControllerInput.BUTTON_Y:
                ONButtonYRelease?.Invoke();
                break;
            case typeControllerInput.BUMPER_RB:
                ONBumperRBRelease?.Invoke();
                break;
            case typeControllerInput.BUMPER_LB:
                ONBumperLBRelease?.Invoke();
                break;
            case typeControllerInput.BUTTON_START:
                ONButtonStartRelease?.Invoke();
                break;
            case typeControllerInput.BUTTON_BACK:
                ONButtonBackRelease?.Invoke();
                break;
            case typeControllerInput.BUTTON_STICK_LEFT:
                ONButtonStickLeftRelease?.Invoke();
                break;
            case typeControllerInput.BUTTON_STICK_RIGHT:
                ONButtonStickRightRelease?.Invoke();
                break;
            default:
                break;
        }
    }

    protected void CallHold(typeControllerInput _typeControllerInput, typeDirection _typeDirection = typeDirection.None)
    {
        //Debug.Log("Hold");
        switch (_typeControllerInput)
        {
            case typeControllerInput.STICK_LEFT:
                ONStickLeftHold?.Invoke(_typeDirection);
                break;
            case typeControllerInput.STICK_RIGHT:
                ONStickRightHold?.Invoke(_typeDirection);
                break;
            case typeControllerInput.CROSS:
                ONCrossHold?.Invoke(_typeDirection);
                break;
            case typeControllerInput.TRIGGER_LEFT:
                ONTriggerLeftHold?.Invoke();
                break;
            case typeControllerInput.TRIGGER_RIGHT:
                ONTriggerRightHold?.Invoke();
                break;
            case typeControllerInput.BUTTON_A:
                ONButtonAHold?.Invoke();
                break;
            case typeControllerInput.BUTTON_B:
                ONButtonBHold?.Invoke();
                break;
            case typeControllerInput.BUTTON_X:
                ONButtonXHold?.Invoke();
                break;
            case typeControllerInput.BUTTON_Y:
                ONButtonYHold?.Invoke();
                break;
            case typeControllerInput.BUMPER_RB:
                ONBumperRBHold?.Invoke();
                break;
            case typeControllerInput.BUMPER_LB:
                ONBumperLBHold?.Invoke();
                break;
            case typeControllerInput.BUTTON_START:
                ONButtonStartHold?.Invoke();
                break;
            case typeControllerInput.BUTTON_BACK:
                ONButtonBackHold?.Invoke();
                break;
            case typeControllerInput.BUTTON_STICK_LEFT:
                ONButtonStickLeftHold?.Invoke();
                break;
            case typeControllerInput.BUTTON_STICK_RIGHT:
                ONButtonStickRightHold?.Invoke();
                break;
            default:
                break;
        }
    }
    #endregion

    #region Unity Start & Update

    public void KapyStart(GameManager _GM)
    {
        GM = _GM;

        GM.ONManagersInitialized += Init;
    }

    private void OnDestroy()
    {
        GM.ONManagersInitialized -= Init;
    }

    public void Init()
    {
        config = Resources.Load<InputManagerConfig>("Input/InputConfig");
        XBoxControllerInspector.Init();

        KapyInputMap map = AddInputMap<IngameInputMap>();
        AddInputMap<TargetInputMap>();
        AddInputMap<InGameMenuMap>();
        AddInputMap<LobbyMap>();
        if (map != null) map.CustomEnable();
        IsInit = true;
    }



    // Update is called once per frame
    void Update()
    {
        if (IsInit)
        {
            XBoxControllerInspector.Update();
        }
    }
    #endregion

    public KapyInputMap AddInputMap<T>() where T : KapyInputMap
    {
        if (!inputMaps.ContainsKey(typeof(T)))
        {
            GameObject go = new GameObject("$" + typeof(T).Name);
            go.transform.parent = gameObject.transform;
            inputMaps[typeof(T)] = go.AddComponent<T>();
            KapyInputMap tmp = go.GetComponent<KapyInputMap>();
            tmp.IM = this;

            if (currentInputMap == null)
            {
                currentInputMap = tmp;
            }

            return tmp;
        }
        return null;
    }

    public void ChangeInputMap<T>(T _nextInputMap) where T : KapyInputMap
    {
        if (inputMaps.ContainsKey(typeof(T)))
        {
            currentInputMap.CustomDisable();
            _nextInputMap.CustomEnable();
            currentInputMap = _nextInputMap;
        }
    }

    public T ChangeInputMap<T>() where T : KapyInputMap
    {
        if (inputMaps.ContainsKey(typeof(T)))
        {
            currentInputMap.CustomDisable();
            KapyInputMap tmp = inputMaps[typeof(T)];
            tmp.CustomEnable();
            currentInputMap = inputMaps[typeof(T)];
            return tmp as T;
        }
        return null;
    }


    public T GetInputMap<T>() where T : KapyInputMap
    {
        if (inputMaps.ContainsKey(typeof(T)))
        {
            return inputMaps[typeof(T)] as T;
        }
        return null;
    }
}
