let Common = system.getScript("/ti/driverlib/Common.js");

/*
    *  ======== getModeIndex ========
    * Get endpoint code for codegen
    *
    *  @param num - endpoint number
    *  @param type - endpoint type (input, output)
    *
    *  @returns hex value string for EP
    */
function getEndpointCode(num,type){
    const epInStart = 0x80;
    const epOutStart = 0x0;
    return parseInt(num) + ((type=="input")?(epInStart):(epOutStart));
}
function getEndpointInCode(inst){
    return getEndpointCodeFromInst(inst,"input");
}
function getEndpointOutCode(inst){
    return getEndpointCodeFromInst(inst,"output");
}

function getEndpointNumber(inst,type){
    let splitStr = (type=="input")?"_EP_IN":"_EP_OUT";
    try{
        return inst.peripheral.$solution.peripheralName.split(splitStr)[1]; // USBFS0_EP_IN2 => [ USBFS0, 2]
    }
    catch(e){
        return undefined;
    }
}
function getEndpointInstance(inst,type){
    let splitStr = (type=="input")?"_EP_IN":"_EP_OUT";
    try{
        return inst.peripheral.$solution.peripheralName.split(splitStr)[0]; // USBFS0_EP_IN2 => [ USBFS0, 2]
    }
    catch(e){
        return undefined;
    }
}
function getEndpointCodeFromInst(inst,type){
    let num = getEndpointNumber(inst,type);
    return getEndpointCode(num,type);
}

function getCDCInstances(inst){
    let mode = inst.mode;
    let cdcInsts = (mode=="device") ? inst.associated_devCDC : inst.associated_hostCDC;
    return cdcInsts;
}

function getHIDInstances(inst){
    let mode = inst.mode;
    let hidInsts = (mode=="device") ? inst.associated_devHID : inst.associated_hostHID;
    return hidInsts;
}

function getAudioInstances(inst){
    let mode = inst.mode;
    let audioInsts  = (mode=="device") ? inst.associated_devAudio : [];
    if(_.isUndefined(audioInsts)){ audioInsts = []; } // Prevent errors when disabled
    return audioInsts;
}

function getMSCInstances(inst){
    let mode = inst.mode;
    let mscInsts = (mode=="device") ? inst.associated_devMSC : inst.associated_hostMSC;
    return mscInsts;
}

function getBillboardInstances(inst){
    let mode = inst.mode;
    let billboardInsts = (mode=="device") ? inst.associated_devBillboard : [];
    if(_.isUndefined(billboardInsts)){ billboardInsts = []; } // Prevent errors when disabled
    return billboardInsts;
}

function getClassStringConfigTemplate(inst,ui){
    return [
        {
            name: "overrideDefaultStr",
            displayName: "Override Default String",
            default: false,
            longDescription: `
Selecting this option allows the user tooverride the default class string, which
is tied to the class instance name`,
            onChange: (inst,ui)=>{
                ui.defaultClassString.hidden = inst.overrideDefaultStr;
                ui.classString.hidden = !inst.overrideDefaultStr;
                inst.classString = inst.defaultClassString;
            }
        },
        {
            name: "defaultClassString",
            displayName: "Class String",
            longDescription: "Class string for usb descriptor",
            default: "",
            getValue: (inst)=>{
                return inst.$name;
            }
        },
        {
            name: "classString",
            displayName: "Class String",
            longDescription: "Class string for usb descriptor",
            default: "TinyUSB",
            hidden: true,
        },
    ];
}
function getEndpointConfigTemplate(inst,ui){
    return [
        {
            name: "GROUP_ENDPOINTS",
            displayName: "Endpoint Configuration",
            config: [],
        }
    ];
}

function validateClassStr(inst,validation){
    if(inst.overrideDefaultStr){
        if(!Common.isValidInputStr(inst.classString)){
            validation.logError("Invalid Class String", inst, "classString");
        }
    }
}

function getGenericBufferSizeConfig(){
    return [
        {
            name: "epBufferSize",
            default: 0,
            isInteger: true,
            hidden: true,
        }
    ];
}


exports = {
    getEndpointCode : getEndpointCode,
    getEndpointInCode : getEndpointInCode,
    getEndpointOutCode : getEndpointOutCode,
    getEndpointCodeFromInst : getEndpointCodeFromInst,
    getCDCInstances: getCDCInstances,
    getHIDInstances: getHIDInstances,
    getAudioInstances: getAudioInstances,
    getMSCInstances: getMSCInstances,
    getBillboardInstances: getBillboardInstances,
    getGenericBufferSizeConfig: getGenericBufferSizeConfig,
    getClassStringConfigTemplate: getClassStringConfigTemplate,
    getEndpointConfigTemplate: getEndpointConfigTemplate,
    validateClassStr: validateClassStr,
};
