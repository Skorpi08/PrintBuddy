#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "../Global/GlobalDataController.h"

/**
 * Webpage form items for reuse
 */
static const char FORM_ITEM_ROW_START[] PROGMEM = "<div class='bx--row' %ROWEXTRACLASS%>";
static const char FORM_ITEM_ROW_EXT[] PROGMEM = "bx--col bx--col--auto";
static const char FORM_ITEM_ROW_END[] PROGMEM = "</div>";

static const char FORM_ITEM_CHECKBOX[] PROGMEM = "<div class='%ROWEXT% bx--form-item' %DIVEXTRACLASS%>"
                        "<input class='bx--toggle-input bx--toggle-input--small' id='%FORMID%' type='checkbox' name='%FORMID%' %CHECKED% %ONCHANGE%>"
                        "<label class='bx--toggle-input__label' for='%FORMID%'>"
                            "<span class='bx--toggle__switch'>"
                                "<svg class='bx--toggle__check' width='6px' height='5px' viewBox='0 0 6 5'>"
                                    "<path d='M2.2 2.7L5 0 6 1 2.2 5 0 2.7 1 1.5z' />"
                                "</svg>"
                                "<span class='bx--toggle__text--off' aria-hidden='true'>%LABELOFF%</span>"
                                "<span class='bx--toggle__text--on' aria-hidden='true'>%LABELON%</span>"
                            "</span>"
                        "</label>"
                    "</div>";
static const char FORM_ITEM_CHECKBOX_ON[] PROGMEM = " activated";
static const char FORM_ITEM_CHECKBOX_OFF[] PROGMEM = " deactivated";

static const char FORM_ITEM_INPUT[] PROGMEM = "<div class='%ROWEXT% bx--form-item' %DIVEXTRACLASS%>"
                            "<label for='%FORMID%' class='bx--label'>%LABEL%</label>"
                            "<input id='%FORMID%' type='%FIELDTYPE%' class='bx--text-input' placeholder='%PLACEHOLDER%' name='%FORMID%' value='%VALUE%' maxlength='%MAXLEN%' %EVENTS%>"
                        "</div>";

static const char FORM_ITEM_SELECT_START[] PROGMEM = "<div class='bx--form-item bx--select %ROWEXT%' %DIVEXTRACLASS%>"
                            "<label for='%FORMID%' class='bx--label'>%LABEL%</label>"
                            "<div class='bx--select-input__wrapper'>"
                                "<select id='%FORMID%' class='bx--select-input' name='%FORMID%' %EVENTS% data-preselect='%PRESELECTVAL%'>";
static const char FORM_ITEM_SELECT_END[] PROGMEM = "</select>"
                                "<svg focusable='false' preserveAspectRatio='xMidYMid meet' style='will-change: transform;' xmlns='http://www.w3.org/2000/svg' class='bx--select__arrow' width='10' height='6' viewBox='0 0 10 6' aria-hidden='true'><path d='M5 6L0 1 0.7 0.3 5 4.6 9.3 0.3 10 1z'></path></svg>"
                            "</div>"
                        "</div>";

static const char FORM_ITEM_SUBMIT[] PROGMEM = "<div class='bx--form-item %ROWEXT%' %DIVEXTRACLASS%>"
                        "<button class='bx--btn bx--btn--primary' type='submit'>Save</button>"
                    "</div>";




static const char ICON32_TEMP[] PROGMEM = "<svg id='icon' xmlns='http://www.w3.org/2000/svg' viewBox='0 0 32 32' width='32' height='32' fill='currentColor'>"
                                            "<path d='M13,17.26V6A4,4,0,0,0,5,6V17.26a7,7,0,1,0,8,0ZM9,4a2,2,0,0,1,2,2v7H7V6A2,2,0,0,1,9,4ZM9,28a5,5,0,0,1-2.5-9.33l.5-.28V15h4v3.39l.5.28A5,5,0,0,1,9,28Z' transform='translate(0 0)'/><rect x='20' y='4' width='10' height='2'/><rect x='20' y='10' width='7' height='2'/><rect x='20' y='16' width='10' height='2'/><rect x='20' y='22' width='7' height='2'/>"
                                        "</svg>";

static const char ICON16_WIND[] PROGMEM = "<svg id='icon' xmlns='http://www.w3.org/2000/svg' width='16' height='16' viewBox='0 0 32 32' fill='currentColor'>"
                                            "<path d='M21,15H8V13H21a3,3,0,1,0-3-3H16a5,5,0,1,1,5,5Z'/><path d='M23,28a5.0057,5.0057,0,0,1-5-5h2a3,3,0,1,0,3-3H4V18H23a5,5,0,0,1,0,10Z'/>"
                                        "</svg>";
                                                    
static const char ICON16_HUMIDITY[] PROGMEM = "<svg id='icon' xmlns='http://www.w3.org/2000/svg' width='16' height='16' viewBox='0 0 32 32' fill='currentColor'>"
                                            "<path d='M23.4761,13.9932,16.8472,3.4365a1.04,1.04,0,0,0-1.6944,0L8.4941,14.0444A9.9861,9.9861,0,0,0,7,19a9,9,0,0,0,18,0A10.0632,10.0632,0,0,0,23.4761,13.9932ZM16,26.0005a7.0089,7.0089,0,0,1-7-7,7.978,7.978,0,0,1,1.2183-3.9438l.935-1.4888L21.2271,23.6411A6.9772,6.9772,0,0,1,16,26.0005Z'/>"
                                        "</svg>";

static const char ICON16_PRESSURE[] PROGMEM = "<svg id='icon' xmlns='http://www.w3.org/2000/svg' width='16' height='16' viewBox='0 0 32 32' fill='currentColor'>"
                                            "<path d='M17.5053,16l8.1591-7.2529A1,1,0,0,0,25,7H22V2H20V9h2.37L16,14.6621,9.63,9H12V2H10V7H7a1,1,0,0,0-.6646,1.7471L14.4945,16,6.3353,23.2529A1,1,0,0,0,7,25h3v5h2V23H9.63L16,17.3379,22.37,23H20v7h2V25h3a1,1,0,0,0,.6645-1.7471Z'/>"
                                        "</svg>";

/**
 * Webpage side menu right for main items
 */
static const char MENUE_ITEMS[] PROGMEM = 
    "<li class='cv-switcher-item bx--switcher__item'><a class='cv-switcher-item-link bx--switcher__item-link menitem' href='/'>"
        "Home"
        "<svg focusable='false' preserveAspectRatio='xMidYMid meet' xmlns='http://www.w3.org/2000/svg' fill='currentColor' width='16' height='16' viewBox='0 0 32 32' aria-hidden='true'><path d='M16.6123,2.2138a1.01,1.01,0,0,0-1.2427,0L1,13.4194l1.2427,1.5717L4,13.6209V26a2.0041,2.0041,0,0,0,2,2H26a2.0037,2.0037,0,0,0,2-2V13.63L29.7573,15,31,13.4282ZM18,26H14V18h4Zm2,0V18a2.0023,2.0023,0,0,0-2-2H14a2.002,2.002,0,0,0-2,2v8H6V12.0615l10-7.79,10,7.8005V26Z'></path></svg>"
    "</a></li>"
    "<li class='cv-switcher-item bx--switcher__item'><a class='cv-switcher-item-link bx--switcher__item-link menitem' href='/configureprinter/show'>"
        "Configure Printers"
        "<svg focusable='false' preserveAspectRatio='xMidYMid meet' xmlns='http://www.w3.org/2000/svg' fill='currentColor' width='16' height='16' viewBox='0 0 32 32' aria-hidden='true'><path d='M28,9H25V3H7V9H4a2,2,0,0,0-2,2V21a2,2,0,0,0,2,2H7v6H25V23h3a2,2,0,0,0,2-2V11A2,2,0,0,0,28,9ZM9,5H23V9H9ZM23,27H9V17H23Zm5-6H25V15H7v6H4V11H28Z'></path></svg>"
    "</a></li>"
    "<li class='cv-switcher-item bx--switcher__item'><a class='cv-switcher-item-link bx--switcher__item-link menitem' href='/configurestation/show'>"
        "Configure Station"
        "<svg focusable='false' preserveAspectRatio='xMidYMid meet' xmlns='http://www.w3.org/2000/svg' fill='currentColor' width='16' height='16' viewBox='0 0 32 32' aria-hidden='true'><path d='M27,16.76c0-.25,0-.5,0-.76s0-.51,0-.77l1.92-1.68A2,2,0,0,0,29.3,11L26.94,7a2,2,0,0,0-1.73-1,2,2,0,0,0-.64.1l-2.43.82a11.35,11.35,0,0,0-1.31-.75l-.51-2.52a2,2,0,0,0-2-1.61H13.64a2,2,0,0,0-2,1.61l-.51,2.52a11.48,11.48,0,0,0-1.32.75L7.43,6.06A2,2,0,0,0,6.79,6,2,2,0,0,0,5.06,7L2.7,11a2,2,0,0,0,.41,2.51L5,15.24c0,.25,0,.5,0,.76s0,.51,0,.77L3.11,18.45A2,2,0,0,0,2.7,21L5.06,25a2,2,0,0,0,1.73,1,2,2,0,0,0,.64-.1l2.43-.82a11.35,11.35,0,0,0,1.31.75l.51,2.52a2,2,0,0,0,2,1.61h4.72a2,2,0,0,0,2-1.61l.51-2.52a11.48,11.48,0,0,0,1.32-.75l2.42.82a2,2,0,0,0,.64.1,2,2,0,0,0,1.73-1L29.3,21a2,2,0,0,0-.41-2.51ZM25.21,24l-3.43-1.16a8.86,8.86,0,0,1-2.71,1.57L18.36,28H13.64l-.71-3.55a9.36,9.36,0,0,1-2.7-1.57L6.79,24,4.43,20l2.72-2.4a8.9,8.9,0,0,1,0-3.13L4.43,12,6.79,8l3.43,1.16a8.86,8.86,0,0,1,2.71-1.57L13.64,4h4.72l.71,3.55a9.36,9.36,0,0,1,2.7,1.57L25.21,8,27.57,12l-2.72,2.4a8.9,8.9,0,0,1,0,3.13L27.57,20Z'></path><path d='M16,22a6,6,0,1,1,6-6A5.94,5.94,0,0,1,16,22Zm0-10a3.91,3.91,0,0,0-4,4,3.91,3.91,0,0,0,4,4,3.91,3.91,0,0,0,4-4A3.91,3.91,0,0,0,16,12Z'></path></svg>"
    "</a></li>"
    "<li class='cv-switcher-item bx--switcher__item'><a class='cv-switcher-item-link bx--switcher__item-link menitem' href='/configuredisplay/show'>"
        "Configure Display"
        "<svg focusable='false' preserveAspectRatio='xMidYMid meet' xmlns='http://www.w3.org/2000/svg' fill='currentColor' width='16' height='16' viewBox='0 0 32 32' aria-hidden='true'><path d='M28,4H4A2,2,0,0,0,2,6V22a2,2,0,0,0,2,2h8v4H8v2H24V28H20V24h8a2,2,0,0,0,2-2V6A2,2,0,0,0,28,4ZM18,28H14V24h4Zm10-6H4V6H28Z'/></svg>"
    "</a></li>"
    "<li class='cv-switcher-item bx--switcher__item'><a class='cv-switcher-item-link bx--switcher__item-link menitem' href='/configureweather/show'>"
        "Configure Weather"
        "<svg focusable='false' preserveAspectRatio='xMidYMid meet' xmlns='http://www.w3.org/2000/svg' fill='currentColor' width='16' height='16' viewBox='0 0 32 32' aria-hidden='true'><path d='M24.8008,11.1382a8.9938,8.9938,0,0,0-17.6006,0A6.533,6.533,0,0,0,2,17.5H2V19a1,1,0,0,0,1,1H15a1,1,0,0,0,0-2H4v-.4966H4a4.5176,4.5176,0,0,1,4.144-4.4819l.8155-.064.0991-.812a6.9936,6.9936,0,0,1,13.8838,0l.0986.812.8154.064A4.4962,4.4962,0,0,1,23.5,22H7a1,1,0,0,0,0,2H23.5a6.4963,6.4963,0,0,0,1.3008-12.8618Z'></path><rect width='18' height='2' x='2' y='26' rx='1'></rect></svg>"
    "</a></li>"
    "<li class='cv-switcher-item bx--switcher__item'><a class='cv-switcher-item-link bx--switcher__item-link menitem' href='/configuresensor/show'>"
        "Configure Sensor"
        "<svg focusable='false' preserveAspectRatio='xMidYMid meet' xmlns='http://www.w3.org/2000/svg' fill='currentColor' width='16' height='16' viewBox='0 0 32 32' aria-hidden='true'><path d='M30,19H26V15H24v9H8V8l9-.0009V6H13V2H11V6H8A2.002,2.002,0,0,0,6,8v3H2v2H6v6H2v2H6v3a2.0023,2.0023,0,0,0,2,2h3v4h2V26h6v4h2V26h3a2.0027,2.0027,0,0,0,2-2V21h4Z'></path><path d='M26,2a4.0042,4.0042,0,0,0-4,4,3.9556,3.9556,0,0,0,.5668,2.0192L19.5859,11H11V21H21V12.4141l2.9808-2.9808A3.9553,3.9553,0,0,0,26,10a4,4,0,0,0,0-8ZM19,19H13V13h6ZM26,8a2,2,0,1,1,2-2A2.0023,2.0023,0,0,1,26,8Z'></path></svg>"
    "</a></li>"
    "<li class='cv-switcher-item bx--switcher__item'><a class='cv-switcher-item-link bx--switcher__item-link menitem' onclick='openModal(\"resetSettingsModal\")'>"
        "Reset Settings"
        "<svg focusable='false' preserveAspectRatio='xMidYMid meet' xmlns='http://www.w3.org/2000/svg' fill='currentColor' width='16' height='16' viewBox='0 0 32 32' aria-hidden='true'><path d='M18,28A12,12,0,1,0,6,16v6.2L2.4,18.6,1,20l6,6,6-6-1.4-1.4L8,22.2V16H8A10,10,0,1,1,18,26Z'></path></svg>"
    "</a></li>"
    "<li class='cv-switcher-item bx--switcher__item'><a class='cv-switcher-item-link bx--switcher__item-link menitem' onclick='openModal(\"resetWifiModal\")'>"
        "Forget WiFi"
        "<svg focusable='false' preserveAspectRatio='xMidYMid meet' xmlns='http://www.w3.org/2000/svg' fill='currentColor' width='16' height='16' viewBox='0 0 32 32' aria-hidden='true'><circle cx='16' cy='25' r='2'></circle><path d='M30 3.4141L28.5859 2 2 28.5859 3.4141 30 14.0962 19.3179a5.9359 5.9359 0 016.01 1.3193L21.52 19.2236a7.9669 7.9669 0 00-5.125-2.2041l3.3875-3.3877a11.9908 11.9908 0 014.5647 2.7647L25.76 14.9829A13.975 13.975 0 0021.334 12.08L24.3308 9.083a17.9364 17.9364 0 014.2546 3.0747L30 10.7432v-.002a20.02 20.02 0 00-4.1895-3.1377zM14.68 13.0776l2.0415-2.0415C16.481 11.0234 16.2437 11 16 11a13.9447 13.9447 0 00-9.771 3.9927l1.4136 1.4136A11.97 11.97 0 0114.68 13.0776zM16 7a17.87 17.87 0 014.2324.5254L21.875 5.8828A19.9537 19.9537 0 002 10.7412v.0225L3.4043 12.168A17.9193 17.9193 0 0116 7z'></path></svg>"
    "</a></li>"
    "<li class='cv-switcher-item bx--switcher__item'><a class='cv-switcher-item-link bx--switcher__item-link menitem' href='/update'>"
        "Firmware Update"
        "<svg focusable='false' preserveAspectRatio='xMidYMid meet' xmlns='http://www.w3.org/2000/svg' fill='currentColor' width='16' height='16' viewBox='0 0 32 32' aria-hidden='true'><path d='M28,12H20V4h8Zm-6-2h4V6H22Z'></path><path d='M17,15V9H9V23H23V15Zm-6-4h4v4H11Zm4,10H11V17h4Zm6,0H17V17h4Z'></path><path d='M26,28H6a2.0023,2.0023,0,0,1-2-2V6A2.0023,2.0023,0,0,1,6,4H16V6H6V26H26V16h2V26A2.0023,2.0023,0,0,1,26,28Z'></path></svg>"
    "</a></li>"
    "<li class='cv-switcher-item bx--switcher__item'><a class='cv-switcher-item-link bx--switcher__item-link menitem' href='https://github.com/Qrome' target='_blank'>"
        "About"
        "<svg focusable='false' preserveAspectRatio='xMidYMid meet' xmlns='http://www.w3.org/2000/svg' fill='currentColor' width='16' height='16' viewBox='0 0 32 32' aria-hidden='true'><path d='M16,2A14,14,0,1,0,30,16,14,14,0,0,0,16,2Zm0,26A12,12,0,1,1,28,16,12,12,0,0,1,16,28Z'></path><circle cx='16' cy='23.5' r='1.5'></circle><path d='M17,8H15.5A4.49,4.49,0,0,0,11,12.5V13h2v-.5A2.5,2.5,0,0,1,15.5,10H17a2.5,2.5,0,0,1,0,5H15v4.5h2V17a4.5,4.5,0,0,0,0-9Z'></path></svg>"
    "</a></li>";

/**
 * Basic header/footer blocks
 */
static const char HEADER_BLOCK1[] PROGMEM = "<!DOCTYPE HTML>"
        "<html><head><title>PrintBuddy</title><link rel='icon' href='data:;base64,='>"
        "<meta charset='UTF-8'>"
        "<meta name='viewport' content='width=device-width, initial-scale=1'>";

static const char HEADER_BLOCK2[] PROGMEM = "<link rel='stylesheet' href='https://www.w3schools.com/w3css/4/w3.css'>"
        "<link rel='stylesheet' href='https://unpkg.com/carbon-components/css/carbon-components.min.css'></style>"
        "<link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.15.1/css/all.css'>"
        "<link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/open-weather-icons@0.0.8/dist/css/open-weather-icons.css'>"
        "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js'></script>"
        "<script src='https://cdnjs.cloudflare.com/ajax/libs/moment.js/2.29.1/moment-with-locales.min.js' crossorigin='anonymous'></script>"
        "<script src='https://cdnjs.cloudflare.com/ajax/libs/moment-timezone/0.5.32/moment-timezone-with-data.js' crossorigin='anonymous'></script>"
        "<style>.hidden{display:none} .bx--form-item{margin-bottom:20px} .bx--table-column-menu{width: 3.25rem} .menitem{padding:6px 1rem;font-size:.875rem;font-weight:600;line-height:1.29;letter-spacing:.16px;display:flex;justify-content:space-between;text-decoration:none;color:#c6c6c6} .pStateBar{width:100%;background-color:#ddd;margin:5px 0} .pStateBarD{height:20px;background-color:#24a148;text-align:center!important;font-size:13px!important;color:#ffffff;padding:4px 0} .pStateHr{border-top:1px solid #0043ce;margin: 5px 0}</style>"
        "<script>function showhide(a,b) {var e=$(\"[data-sh='\"+b+\"']\");var f=$(\"#\" + a);if (f.checked||f.prop('checked')){e.removeClass('hidden');}else{e.addClass('hidden');}}</script>"
        "<script>function showhideDir(a,b) {var e=$(\"[data-sh='\"+a+\"']\");var f=$(\"#\" + a);if (b){e.removeClass('hidden');}else{e.addClass('hidden');}}</script>"
        "<script>function openModal(refelementId){document.body.classList.add(\"bx--body--with-modal-open\");document.getElementById(refelementId).classList.add(\"is-visible\")} function closeModal(refelementId){document.getElementById(refelementId).classList.remove(\"is-visible\");document.body.classList.remove(\"bx--body--with-modal-open\")}</script>"
        "<script>function isNumberKey(e){var h=e.which?e.which:event.keyCode;return!(h>31&&(h<48||h>57))}</script>"
        "<script>function openUrl(e){window.location.assign(e)}</script>"
        "<script>function apiTypeSelect(r,t){if($(\"#\"+r).find(\":selected\").data('need-api')){$(\"[data-sh='\"+t+\"']\").removeClass('hidden')}else{$(\"[data-sh='\"+t+\"']\").addClass('hidden')}}</script>"
        "</head><body>"
        "<header class='cv-header bx--header'>"
        "<a href='/' class='cv-header-name bx--header__name'>";

static const char HEADER_BLOCK3[] PROGMEM = "</a>"
        "<nav class='cv-header-nav bx--header__nav'></nav>"
        "<div class='bx--header__global'>"
            "<button type='button' class='cv-header-global-action bx--header__action' onclick='openChipInfo()'>"
                "<svg focusable='false' preserveAspectRatio='xMidYMid meet' xmlns='http://www.w3.org/2000/svg' fill='currentColor' width='20' height='20' viewBox='0 0 32 32' aria-hidden='true'>"
                    "<path d='M11,11V21H21V11Zm8,8H13V13h6Z'></path>"
                    "<path d='M30,13V11H26V8a2,2,0,0,0-2-2H21V2H19V6H13V2H11V6H8A2,2,0,0,0,6,8v3H2v2H6v6H2v2H6v3a2,2,0,0,0,2,2h3v4h2V26h6v4h2V26h3a2,2,0,0,0,2-2V21h4V19H26V13ZM24,24H8V8H24Z'></path>"
                "</svg>"
            "</button>"
            "<button type='button' class='cv-header-global-action bx--header__action' onclick='openSidebar()'>"
                "<svg focusable='false' preserveAspectRatio='xMidYMid meet' xmlns='http://www.w3.org/2000/svg' fill='currentColor' width='20' height='20' viewBox='0 0 32 32' aria-hidden='true'>"
                    "<path d='M14 4H18V8H14zM4 4H8V8H4zM24 4H28V8H24zM14 14H18V18H14zM4 14H8V18H4zM24 14H28V18H24zM14 24H18V28H14zM4 24H8V28H4zM24 24H28V28H24z'></path>"
                "</svg>"
            "</button>"
        "</div>"
        "<div id='sidebar' class='cv-header-panel bx--header-panel'>"
            "<ul class='cv-switcher bx--switcher__item'>";

static const char HEADER_BLOCK4[] PROGMEM = "</ul>"
        "</div>"
        "<div class='bx--toast-notification bx--toast-notification--info hidden' style='position: absolute; right: -16px; top: 40px;' id='chipinfo'>"
            "<div class='bx--toast-notification__details'>"
                "<h3 class='bx--toast-notification__title'>ESP Details</h3>"
                    "<div class='bx--toast-notification__subtitle'>";

static const char HEADER_BLOCK5[] PROGMEM = "</div>"
                "</div>"
            "</div>"
        "</header>"
        "<script>function openSidebar(){document.getElementById('sidebar').classList.toggle('bx--header-panel--expanded');document.getElementById('chipinfo').classList.add('hidden');};function openChipInfo(){document.getElementById('sidebar').classList.remove('bx--header-panel--expanded');document.getElementById('chipinfo').classList.toggle('hidden');}</script>"
        "<br><div class='bx--grid bx--grid--full-width' style='margin-top:60px'>"
            "<div class='page-header' style='margin-bottom:20px;position:relative'><h4 class='page-header__label'>";

static const char HEADER_BLOCK_ERROR[] PROGMEM = "<div class='bx--inline-notification bx--inline-notification--error' role='alert' style='max-width:100%'>"
            "<div class='bx--inline-notification__details'>"
                "<svg focusable='false' preserveAspectRatio='xMidYMid meet' style='will-change: transform;' xmlns='http://www.w3.org/2000/svg' class='bx--inline-notification__icon' width='20' height='20' viewBox='0 0 20 20' aria-hidden='true'><path d='M10,1c-5,0-9,4-9,9s4,9,9,9s9-4,9-9S15,1,10,1z M13.5,14.5l-8-8l1-1l8,8L13.5,14.5z'></path><path d='M13.5,14.5l-8-8l1-1l8,8L13.5,14.5z' data-icon-path='inner-path' opacity='0'></path></svg>"
                "<div class='bx--inline-notification__text-wrapper'>"
                    "<p class='bx--inline-notification__title'>%ERRORMSG%</p>"
                "</div>"
            "</div>"
        "</div>";

static const char HEADER_BLOCK_OK[] PROGMEM = "<div class='bx--inline-notification bx--inline-notification--success' role='alert' style='max-width:100%'>"
            "<div class='bx--inline-notification__details'>"
                "<svg focusable='false' preserveAspectRatio='xMidYMid meet' style='will-change: transform;' xmlns='http://www.w3.org/2000/svg' class='bx--inline-notification__icon' width='20' height='20' viewBox='0 0 20 20' aria-hidden='true'><path d='M10,1c-4.9,0-9,4.1-9,9s4.1,9,9,9s9-4,9-9S15,1,10,1z M8.7,13.5l-3.2-3.2l1-1l2.2,2.2l4.8-4.8l1,1L8.7,13.5z'></path><path fill='none' d='M8.7,13.5l-3.2-3.2l1-1l2.2,2.2l4.8-4.8l1,1L8.7,13.5z' data-icon-path='inner-path' opacity='0'></path></svg>"
                "<div class='bx--inline-notification__text-wrapper'>"
                    "<p class='bx--inline-notification__title'>%OKMSG%</p>"
                "</div>"
            "</div>"
        "</div>";

static const char FOOTER_BLOCK[] PROGMEM = "<br><br><br></div>"
        "<div class='bx--loading-overlay hidden' id='pageloading'>"
            "<div data-loading class='bx--loading'>"
                "<svg class='bx--loading__svg' viewBox='-75 -75 150 150'>"
                    "<title>Loading</title>"
                    "<circle class='bx--loading__stroke' cx='0' cy='0' r='37.5' />"
                "</svg>"
            "</div>"
        "</div>"
        "<script src='https://unpkg.com/carbon-components/scripts/carbon-components.min.js'></script>"
        "<script>$(function(){$('form').on('submit',function(e){$('#pageloading').removeClass('hidden')});$(\"input[type='checkbox']\").trigger('change')})</script>"
    "</body>"
"</html>";


/**
 * Global Text
 */
static const char GLOBAL_TEXT_WARNING[] PROGMEM = "WARNING";
static const char GLOBAL_TEXT_ABORT[] PROGMEM = "Abort";
static const char GLOBAL_TEXT_RESET[] PROGMEM = "Reset";
static const char GLOBAL_TEXT_DELETE[] PROGMEM = "Delete";
static const char GLOBAL_TEXT_TRESET[] PROGMEM = "Reset settings";
static const char GLOBAL_TEXT_CRESET[] PROGMEM = "Do you want to reset to default settings?";
static const char GLOBAL_TEXT_TFWIFI[] PROGMEM = "Reset wifi";
static const char GLOBAL_TEXT_CFWIFI[] PROGMEM = "Do you want to reset wifi to default settings?";
static const char GLOBAL_TEXT_TDPRINTER[] PROGMEM = "Delete printer";
static const char GLOBAL_TEXT_CDPRINTER[] PROGMEM = "Do you want to delete the printer configuration &quot;%PRINTERNAME%&quot;?";

/**
 * Controls for update firmware/filesystem
 */
static const char UPDATE_FORM[] PROGMEM = "<div class='bx--row'>"
    "<div class='bx--col-md-4'>"
        "<form method='POST' action='' enctype='multipart/form-data'>"
            "<div class='cv-file-uploader cv-form-item bx--form-item'>"
                "<strong class='bx--file--label'>Update Firmware</strong>"
                "<p class='bx--label-description'>Select the firmware you want to upload</p>"
                "<div data-file='' class='bx--file'>"
                    "<label for='firmware' role='button' tabindex='0' class='bx--file-browse-btn'>"
                        "<div data-file-drop-container='' class='bx--file__drop-container'>"
                            "Drag and drop file here or upload"
                            "<input type='file' id='firmware' accept='.bin,.bin.gz' class='bx--file-input' name='firmware' onchange='document.getElementById(\"ffile\").innerHTML = \"\"'>"
                        "</div>"
                    "</label>"
                    "<div data-file-container='' class='bx--file-container' id='ffile'></div>"
                "</div>"
            "</div>"
            "<input type='submit' value='Update Firmware' class='bx--btn bx--btn--danger'>"
        "</form>"
    "</div>"
    "<div class='bx--col-md-4'>"
        "<form method='POST' action='' enctype='multipart/form-data'>"
            "<div class='cv-file-uploader cv-form-item bx--form-item'>"
                "<strong class='bx--file--label'>Update FileSystem</strong>"
                "<p class='bx--label-description'>Select the filesystem you want to upload</p>"
                "<div data-file='' class='bx--file'>"
                    "<label for='filesystem' role='button' tabindex='0' class='bx--file-browse-btn'>"
                        "<div data-file-drop-container='' class='bx--file__drop-container'>"
                            "Drag and drop file here or upload"
                            "<input type='file' id='filesystem' accept='.bin,.bin.gz' class='bx--file-input' name='filesystem' onchange='document.getElementById(\"fsys\").innerHTML = \"\"'>"
                        "</div>"
                    "</label>"
                    "<div data-file-container='' class='bx--file-container' id='fsys'></div>"
                "</div>"
            "</div>"
            "<input type='submit' value='Update FileSystem' class='bx--btn bx--btn--danger'>"
        "</form>"
    "</div>"
"</div>";

/**
 * Controls for weather configuration
 */
static const char WEATHER_FORM_START[] PROGMEM = "<form action='/configureweather/update' method='get'>";

static const char WEATHER_FORM1_ID[] PROGMEM = "isWeatherEnabled";
static const char WEATHER_FORM1_LABEL[] PROGMEM = "Display Weather when printer is off";

static const char WEATHER_FORM2_ID[] PROGMEM = "metric";
static const char WEATHER_FORM2_LABEL_ON[] PROGMEM = "Show in Celsius";
static const char WEATHER_FORM2_LABEL_OFF[] PROGMEM = "Show in Fahrenheit";

static const char WEATHER_FORM3_ID[] PROGMEM = "openWeatherMapApiKey";
static const char WEATHER_FORM3_LABEL[] PROGMEM = "OpenWeatherMap API Key (get from <a href='https://openweathermap.org/' target='_BLANK'>here</a>)";

static const char WEATHER_FORM4_ID[] PROGMEM = "city1";
static const char WEATHER_FORM4_LABEL[] PROGMEM = "(<a href='http://openweathermap.org/find' target='_BLANK'><i class='fa fa-search'></i> Search for City ID</a>)";

static const char WEATHER_FORM5_ID[] PROGMEM = "language";
static const char WEATHER_FORM5_LABEL[] PROGMEM = "Weather Language";
static const char WEATHER_FORM5_OPTIONS[] PROGMEM = "<option class='bx--select-option'>ar</option>"
                                    "<option class='bx--select-option'>bg</option>"
                                    "<option class='bx--select-option'>ca</option>"
                                    "<option class='bx--select-option'>cz</option>"
                                    "<option class='bx--select-option'>de</option>"
                                    "<option class='bx--select-option'>el</option>"
                                    "<option class='bx--select-option'>en</option>"
                                    "<option class='bx--select-option'>fa</option>"
                                    "<option class='bx--select-option'>fi</option>"
                                    "<option class='bx--select-option'>fr</option>"
                                    "<option class='bx--select-option'>gl</option>"
                                    "<option class='bx--select-option'>hr</option>"
                                    "<option class='bx--select-option'>hu</option>"
                                    "<option class='bx--select-option'>it</option>"
                                    "<option class='bx--select-option'>ja</option>"
                                    "<option class='bx--select-option'>kr</option>"
                                    "<option class='bx--select-option'>la</option>"
                                    "<option class='bx--select-option'>lt</option>"
                                    "<option class='bx--select-option'>mk</option>"
                                    "<option class='bx--select-option'>nl</option>"
                                    "<option class='bx--select-option'>pl</option>"
                                    "<option class='bx--select-option'>pt</option>"
                                    "<option class='bx--select-option'>ro</option>"
                                    "<option class='bx--select-option'>ru</option>"
                                    "<option class='bx--select-option'>se</option>"
                                    "<option class='bx--select-option'>sk</option>"
                                    "<option class='bx--select-option'>sl</option>"
                                    "<option class='bx--select-option'>es</option>"
                                    "<option class='bx--select-option'>tr</option>"
                                    "<option class='bx--select-option'>ua</option>"
                                    "<option class='bx--select-option'>vi</option>"
                                    "<option class='bx--select-option'>zh_cn</option>"
                                    "<option class='bx--select-option'>zh_tw</option>";

static const char WEATHER_FORM_END[] PROGMEM = "</form>";

/**
 * Controls for station configuration
 */
static const char STATION_CONFIG_FORM_START[] PROGMEM = "<form action='/configurestation/update' method='get'>";

static const char STATION_CONFIG_FORM1_ID[] PROGMEM = "isClockEnabled";
static const char STATION_CONFIG_FORM1_LABEL[] PROGMEM = "Display Clock when printer is off";

static const char STATION_CONFIG_FORM2_ID[] PROGMEM = "is24hour";
static const char STATION_CONFIG_FORM2_LABEL[] PROGMEM = "Use 24 Hour Clock (military time)";

static const char STATION_CONFIG_FORM4_ID[] PROGMEM = "useFlash";
static const char STATION_CONFIG_FORM4_LABEL[] PROGMEM = "Flash System LED on Service Call";

static const char STATION_CONFIG_FORM5_ID[] PROGMEM = "refresh";
static const char STATION_CONFIG_FORM5_LABEL[] PROGMEM = "Clock Sync / Weather Refresh (minutes)";
static const char STATION_CONFIG_FORM5_OPTIONS[] PROGMEM = "<option class='bx--select-option'>10</option>"
                                "<option class='bx--select-option'>15</option>"
                                "<option class='bx--select-option'>20</option>"
                                "<option class='bx--select-option'>30</option>"
                                "<option class='bx--select-option'>60</option>";

static const char STATION_CONFIG_FORM6_ID[] PROGMEM = "utcoffset";
static const char STATION_CONFIG_FORM6_LABEL[] PROGMEM = "Timezone";

static const char STATION_CONFIG_FORM7_ID[] PROGMEM = "isBasicAuth";
static const char STATION_CONFIG_FORM7_LABEL[] PROGMEM = "Use Security Credentials for Configuration Changes";

static const char STATION_CONFIG_FORM8_ID[] PROGMEM = "userid";
static const char STATION_CONFIG_FORM8_LABEL[] PROGMEM = "User ID (for this interface)";

static const char STATION_CONFIG_FORM9_ID[] PROGMEM = "stationpassword";
static const char STATION_CONFIG_FORM9_LABEL[] PROGMEM = "Password (for this interface)";

static const char STATION_CONFIG_FORM_END[] PROGMEM = "</form><script>showhide('isBasicAuth', 'uspw')</script>"
    "<script>const re=\"/[.](\\d){1,2}[|]/gm\";function hc(s){return btoa(s.split(\"\").reduce(function(a,b){a=((a<<5)-a)+b.charCodeAt(0);return a&a},0));};$(function(){let elem=$(\"select[id=utcoffset]\");elem.find(\"option\").remove();$.each(moment.tz.names(),(i,e)=>{let o=moment.utc().tz(e).format('Z').replace(\":00\", \"\").replace(\"0\", \"\").replace(\"+\", \"\").replace(\":\", \".\")+\"|\"+hc(e).substr(0, 16);if(o.search(re)<0){elem.append(new Option(e,o,elem.data(\"preselect\")==o,elem.data(\"preselect\")==o))}})})</script>";

/**
 * Controls for printer configuration
 */
static const char CONFPRINTER_FORM_START[] PROGMEM = "<div class='bx--row'>"
                "<div class='bx--col bx--col--auto bx--data-table-container' data-table>"
                    "<div class='bx--data-table-header'>"
                        "<h4 class='bx--data-table-header__title'>Printers to monitor</h4>"
                        "<p class='bx--data-table-header__description'>Configurationdata</p>"
                    "</div>"
                    "<section class='bx--table-toolbar'>"
                        "<div class='bx--toolbar-content'>"
                            "<button class='bx--btn bx--btn--sm bx--btn--primary' onclick='openModal(\"mae-0\")'>"
                                "Add new"
                                "<svg focusable='false' preserveAspectRatio='xMidYMid meet' style='will-change: transform;' xmlns='http://www.w3.org/2000/svg' class='bx--btn__icon' width='20' height='20' viewBox='0 0 32 32'><path d='M17 15L17 7 15 7 15 15 7 15 7 17 15 17 15 25 17 25 17 17 25 17 25 15 17 15z'></path></svg>"
                            "</button>"
                        "</div>"
                    "</section>"
                    "<table class='bx--data-table bx--data-table--visible-overflow-menu'>"
                        "<thead>"
                            "<tr>"
                                "<th><span class='bx--table-header-label'>Name</span></th>"
                                "<th><span class='bx--table-header-label'>Type</span></th>"
                                "<th><span class='bx--table-header-label'>State</span></th>"
                                "<th class='bx--table-column-menu' style='width: 3.25rem'></th>"
                            "</tr>"
                        "</thead>"
                        "<tbody>";

static const char CONFPRINTER_FORM_ROW_ERROR[] PROGMEM =  "<div class='bx--tag bx--tag--magenta'>"
                                        "%STATUS%"
                                    "</div>";
static const char CONFPRINTER_FORM_ROW_OK[] PROGMEM =  "<div class='bx--tag bx--tag--green'>"
                                        "%STATUS%"
                                    "</div>";

static const char CONFPRINTER_FORM_ROW[] PROGMEM = "<tr>"
                                "<td>%NAME%</td>"
                                "<td>%TYPE%</td>"
                                "<td>%STATE%</td>"
                                "<td class='bx--table-column-menu' style='width: 3.25rem'>"
                                    "<div data-overflow-menu role='menu' tabindex='0' class='bx--overflow-menu'>"
                                        "<svg focusable='false' preserveAspectRatio='xMidYMid meet' style='will-change: transform;' xmlns='http://www.w3.org/2000/svg' class='bx--overflow-menu__icon' width='16' height='16' viewBox='0 0 16 16' aria-hidden='true'><circle cx='8' cy='3' r='1'></circle><circle cx='8' cy='8' r='1'></circle><circle cx='8' cy='13' r='1'></circle></svg>"
                                        "<ul class='bx--overflow-menu-options bx--overflow-menu--flip' data-floating-menu-direction='bottom'>"
                                            "<li class='bx--overflow-menu-options__option bx--table-row--menu-option'>"
                                                "<button class='bx--overflow-menu-options__btn' onclick='openModal(\"mae-%ID%\")'>"
                                                    "<div class='bx--overflow-menu-options__option-content'>"
                                                        "<svg focusable='false' preserveAspectRatio='xMidYMid meet' style='will-change: transform;' xmlns='http://www.w3.org/2000/svg' width='16' height='16' viewBox='0 0 16 16' aria-hidden='true'><path d='M1 13H15V14H1zM12.7 4.5c.4-.4.4-1 0-1.4 0 0 0 0 0 0l-1.8-1.8c-.4-.4-1-.4-1.4 0 0 0 0 0 0 0L2 8.8V12h3.2L12.7 4.5zM10.2 2L12 3.8l-1.5 1.5L8.7 3.5 10.2 2zM3 11V9.2l5-5L9.8 6l-5 5H3z'></path></svg> "
                                                        "Edit"
                                                    "</div>"
                                                "</button>"
                                            "</li>"
                                            "<li class='bx--overflow-menu-options__option bx--table-row--menu-option'>"
                                                "<button class='bx--overflow-menu-options__btn' onclick='openModal(\"deletePrinterModal-%ID%\")'>"
                                                    "<div class='bx--overflow-menu-options__option-content'>"
                                                        "<svg focusable='false' preserveAspectRatio='xMidYMid meet' style='will-change: transform;' xmlns='http://www.w3.org/2000/svg' width='16' height='16' viewBox='0 0 16 16' aria-hidden='true'><path d='M6 6H7V12H6zM9 6H10V12H9z'></path><path d='M2 3v1h1v10c0 .6.4 1 1 1h8c.6 0 1-.4 1-1V4h1V3H2zM4 14V4h8v10H4zM6 1H10V2H6z'></path></svg> "
                                                        "Delete"
                                                    "</div>"
                                                "</button>"
                                            "</li>"
                                        "</ul>"
                                    "</div>"
                                "</td>"
                            "</tr>";

static const char CONFPRINTER_FORM_END[] PROGMEM = "</tbody>"
                    "</table>"
                "</div>"
            "</div>"
            "<script>$(\"select[id^='e-tapi']\").trigger('change')</script>";


static const char CONFPRINTER_FORM_ADDEDIT_TA[] PROGMEM = "Create new printer";
static const char CONFPRINTER_FORM_ADDEDIT_TE[] PROGMEM = "Edit data for printer";

static const char CONFPRINTER_FORM_ADDEDIT_START[] PROGMEM = "<div data-modal id='mae-%ID%' class='bx--modal' role='dialog' aria-modal='true' aria-labelledby='mae-%ID%-label' aria-describedby='mae-%ID%-heading' tabindex='-1'>"
                "<div class='bx--modal-container'>"
                    "<form method='GET' action='/configureprinter/edit'>"
                        "<input type='hidden' name='id' value='%ID%'>"
                        "<div class='bx--modal-header'>"
                            "<p class='bx--modal-header__label bx--type-delta' id='mae-%ID%-label'>Printer Configuration</p>"
                            "<p class='bx--modal-header__heading bx--type-beta' id='mae-%ID%-heading'>%TITLE%</p>"
                            "<button class='bx--modal-close' type='button' onclick='closeModal(\"mae-%ID%\")'>"
                                "<svg focusable='false' preserveAspectRatio='xMidYMid meet' style='will-change: transform;' xmlns='http://www.w3.org/2000/svg' class='bx--modal-close__icon' width='16' height='16' viewBox='0 0 16 16' aria-hidden='true'><path d='M12 4.7L11.3 4 8 7.3 4.7 4 4 4.7 7.3 8 4 11.3 4.7 12 8 8.7 11.3 12 12 11.3 8.7 8z'></path></svg>"
                            "</button>"
                        "</div>"
                        "<div class='bx--modal-content bx--modal-content--with-form'>";

static const char CONFPRINTER_FORM_ADDEDIT1_ID[] PROGMEM = "e-tname";
static const char CONFPRINTER_FORM_ADDEDIT1_LABEL[] PROGMEM = "Printer Name";
static const char CONFPRINTER_FORM_ADDEDIT1_PH[] PROGMEM = "Your custom name";

static const char CONFPRINTER_FORM_ADDEDIT2_ID[] PROGMEM = "e-tapi";
static const char CONFPRINTER_FORM_ADDEDIT2_LABEL[] PROGMEM = "API Type";

static const char CONFPRINTER_FORM_ADDEDIT3_ID[] PROGMEM = "e-tapikey";
static const char CONFPRINTER_FORM_ADDEDIT3_LABEL[] PROGMEM = "API Key";
static const char CONFPRINTER_FORM_ADDEDIT3_PH[] PROGMEM = "Remote api key to use";

static const char CONFPRINTER_FORM_ADDEDIT4_ID[] PROGMEM = "e-taddr";
static const char CONFPRINTER_FORM_ADDEDIT4_LABEL[] PROGMEM = "Hostname or IP Address (do not include http://)";
static const char CONFPRINTER_FORM_ADDEDIT4_PH[] PROGMEM = "Remote address for printer";

static const char CONFPRINTER_FORM_ADDEDIT5_ID[] PROGMEM = "e-tport";
static const char CONFPRINTER_FORM_ADDEDIT5_LABEL[] PROGMEM = "Port";

static const char CONFPRINTER_FORM_ADDEDIT6_ID[] PROGMEM = "e-tapipw";
static const char CONFPRINTER_FORM_ADDEDIT6_LABEL[] PROGMEM = "Haproxy or basic auth";

static const char CONFPRINTER_FORM_ADDEDIT7_ID[] PROGMEM = "e-tapiuser";
static const char CONFPRINTER_FORM_ADDEDIT7_LABEL[] PROGMEM = "User ID";
static const char CONFPRINTER_FORM_ADDEDIT7_PH[] PROGMEM = "Username for basic auth";

static const char CONFPRINTER_FORM_ADDEDIT8_ID[] PROGMEM = "e-tapipass";
static const char CONFPRINTER_FORM_ADDEDIT8_LABEL[] PROGMEM = "Password";
static const char CONFPRINTER_FORM_ADDEDIT8_PH[] PROGMEM = "Password for basic auth";

static const char CONFPRINTER_FORM_ADDEDIT_END[] PROGMEM = "<br><br></div>"
                        "<div class='bx--modal-content--overflow-indicator'></div>"
                        "<div class='bx--modal-footer'>"
                            "<button class='bx--btn bx--btn--secondary' type='reset' onclick='closeModal(\"mae-%ID%\")'>Abort</button>"
                            "<button class='bx--btn bx--btn--primary' type='submit' onclick='closeModal(\"mae-%ID%\")'>Save</button>"
                        "</div>"
                    "</form>"
                "</div>"
                "<span tabindex='0'></span>"
            "</div>"
        "</div>";

/**
 * Danger modal
 */
static const char MODAL_DANGER[] PROGMEM = "<div data-modal id='%ID%' class='bx--modal bx--modal--danger' role='dialog' aria-modal='true' aria-labelledby='%ID%-label' aria-describedby='%ID%-heading' tabindex='-1'>"
    "<div class='bx--modal-container'>"
        "<div class='bx--modal-header'>"
            "<p class='bx--modal-header__label bx--type-delta' id='%ID%-label'>%LABEL%</p>"
            "<p class='bx--modal-header__heading bx--type-beta' id='%ID%-heading'>%HEADING%</p>"
            "<button class='bx--modal-close' type='button' aria-label='close modal' onclick='closeModal(\"%ID%\")'>"
                "<svg focusable='false' preserveAspectRatio='xMidYMid meet' style='will-change: transform;' xmlns='http://www.w3.org/2000/svg' class='bx--modal-close__icon' width='16' height='16' viewBox='0 0 16 16' aria-hidden='true'><path d='M12 4.7L11.3 4 8 7.3 4.7 4 4 4.7 7.3 8 4 11.3 4.7 12 8 8.7 11.3 12 12 11.3 8.7 8z'></path></svg>"
            "</button>"
        "</div>"
        "<div class='bx--modal-content' >"
            "<p>%CONTENT%</p><br><br><br>"
        "</div>"
        "<div class='bx--modal-content--overflow-indicator'></div>"
        "<div class='bx--modal-footer'>"
            "<button class='bx--btn bx--btn--secondary' type='button' onclick='closeModal(\"%ID%\")'>%SECACTION%</button>"
            "<button class='bx--btn bx--btn--danger' type='button' aria-label='Danger' data-modal-primary-focus %MAINEVENT%>%MAINACTION%</button>"
        "</div>"
    "</div>"
    "<span tabindex='0'></span>"
"</div>";

/**
 * Controls for sensor configuration
 */
static const char SENSOR_CONFIG_FORM_START[] PROGMEM = "<form action='/configuresensor/update' method='get'>";

static const char SENSOR_CONFIG_FORM1_ID[] PROGMEM = "isSensor";
static const char SENSOR_CONFIG_FORM1_LABEL[] PROGMEM = "Internal sensor";

static const char SENSOR_CONFIG_FORM2_ID[] PROGMEM = "isShowDisplay";
static const char SENSOR_CONFIG_FORM2_LABEL[] PROGMEM = "Show on display";

static const char SENSOR_CONFIG_FORM3_ID[] PROGMEM = "s-type";
static const char SENSOR_CONFIG_FORM3_LABEL[] PROGMEM = "Sensor type";

static const char SENSOR_CONFIG_FORM_END[] PROGMEM = "</form>";

/**
 * Controls for display configuration
 */
static const char DISPLAY_CONFIG_FORM_START[] PROGMEM = "<form action='/configuredisplay/update' method='get'>";

static const char DISPLAY_CONFIG_FORM1_ID[] PROGMEM = "d-type";
static const char DISPLAY_CONFIG_FORM1_LABEL[] PROGMEM = "Display type";

static const char DISPLAY_CONFIG_FORM2_ID[] PROGMEM = "invDisp";
static const char DISPLAY_CONFIG_FORM2_LABEL[] PROGMEM = "Flip display orientation";

static const char DISPLAY_CONFIG_FORM3_ID[] PROGMEM = "splitWeather";
static const char DISPLAY_CONFIG_FORM3_LABEL[] PROGMEM = "Show weather and sensor on separate pages";

static const char DISPLAY_CONFIG_FORM4_ID[] PROGMEM = "automaticSwitchEnable";
static const char DISPLAY_CONFIG_FORM4_LABEL[] PROGMEM = "Scroll between printers (can be also controlled via display)";

static const char DISPLAY_CONFIG_FORM5_ID[] PROGMEM = "automaticSwitchActivEnable";
static const char DISPLAY_CONFIG_FORM5_LABEL[] PROGMEM = "Automatic switch to printers when at least one printer goes into printing state";

static const char DISPLAY_CONFIG_FORM6_ID[] PROGMEM = "automaticSwitchDelay";
static const char DISPLAY_CONFIG_FORM6_LABEL[] PROGMEM = "Delay in seconds when scrolling enabled";

static const char DISPLAY_CONFIG_FORM7_ID[] PROGMEM = "automaticOff";
static const char DISPLAY_CONFIG_FORM7_LABEL[] PROGMEM = "Delay in seconds when display should switch off (with touch wakeup) when no printer is printing (0 = always on)";

static const char DISPLAY_CONFIG_FORM_END[] PROGMEM = "</form>"
    "<script>var e=document.getElementById('d-type');var val=e.value;if(val==0){showhideDir('oled',false);showhideDir('nextion',true);}else{showhideDir('oled',true);showhideDir('nextion',false);}</script>";

/**
 * Controls for main page
 */
static const char MAINPAGE_ROW_WEATHER_AND_SENSOR_START[] PROGMEM = "<div class='bx--col bx--col--auto'>"
                "<div class='bx--inline-notification bx--inline-notification--info' style='max-width:100%'>"
                    "<div class='bx--inline-notification__details'>"
                        "<i class='bx--inline-notification__icon owi owi-%ICON%' style='font-size: 90px; margin: 3rem 1rem;'></i>"
                        "<div class='bx--inline-notification__text-wrapper' style='width:100%'>"
                            "<div class='bx--grid bx--grid--full-width' style='width:100%'>"
                                "<div class='bx--row'>";

static const char MAINPAGE_ROW_WEATHER_ERROR_BLOCK[] PROGMEM = "<div class='bx--col bx--col--auto' style='margin: 2rem 0;'>"
                                        "<div class='bx--grid bx--grid--full-width'>"
                                            "<div class='bx--inline-notification bx--inline-notification--error'>"
                                                "<div class='bx--inline-notification__details'>"
                                                    "<div>"
                                                        "<p class='bx--inline-notification__title'>Please <a href='/configureweather/show'>Configure Weather</a></p>"
                                                        "<p class='bx--inline-notification__subtitle'>%ERRORMSG%</p>"
                                                    "</div>"
                                                "</div>"
                                            "</div>"
                                        "</div>"
                                    "</div>";

static const char MAINPAGE_ROW_WEATHER_AND_SENSOR_BLOCK[] PROGMEM = "<div class='bx--col bx--col--auto' style='margin: 2rem 0;'>"
                                        "<div class='bx--grid bx--grid--full-width'>"
                                            "<div class='bx--row'>"
                                                "<div class='bx--col bx--col--auto'>"
                                                    "<h2>%BTITLE% <div class='bx--tag bx--tag--cool-gray'><span class='bx--tag__label'>%BLABEL%</span></div></h2>"
                                                "</div>"
                                            "</div>"
                                            "<div class='bx--row'>"
                                                "<div class='bx--col bx--col--auto'>"
                                                    "<h1>%TEMPICON% %TEMPERATURE%</h1>"
                                                "</div>"
                                            "</div>"
                                            "<div class='bx--row'>"
                                                "<div class='bx--col bx--col--auto'>"
                                                    "%EXTRABLOCK%"
                                                "</div>"
                                            "</div>"
                                            "<div class='bx--row'>"
                                                "<div class='bx--col bx--col--auto'>"
                                                    "%ICONA% %TEXTA%"
                                                    "%ICONB% %TEXTB%"
                                                "</div>"
                                            "</div>"
                                        "</div>"
                                    "</div>";

static const char MAINPAGE_ROW_WEATHER_AND_SENSOR_END[] PROGMEM = "</div>"
                            "</div>"
                        "</div>"
                    "</div>"
                "</div>"
            "</div>";

static const char MAINPAGE_ROW_PRINTER_BLOCK_S_PRINTING[] PROGMEM = "<div class='bx--col bx--col--auto'>"
            "<div class='bx--inline-notification bx--inline-notification--info bx--inline-notification--low-contrast'>"
                "<div class='bx--inline-notification__details'>"
                    "<svg focusable='false' preserveAspectRatio='xMidYMid meet' style='will-change: transform;' xmlns='http://www.w3.org/2000/svg' class='bx--inline-notification__icon' width='20' height='20' viewBox='0 0 32 32' aria-hidden='true'><path d='M16,2A14,14,0,1,0,30,16,14,14,0,0,0,16,2Zm0,5a1.5,1.5,0,1,1-1.5,1.5A1.5,1.5,0,0,1,16,7Zm4,17.12H12V21.88h2.88V15.12H13V12.88h4.13v9H20Z'></path></svg>"
                    "<div style='margin: .5rem 0;width: 100%;'>";

static const char MAINPAGE_ROW_PRINTER_BLOCK_S_ERROROFFLINE[] PROGMEM = "<div class='bx--col bx--col--auto'>"
            "<div class='bx--inline-notification bx--inline-notification--error bx--inline-notification--low-contrast'>"
                "<div class='bx--inline-notification__details'>"
                    "<svg focusable='false' preserveAspectRatio='xMidYMid meet' style='will-change: transform;' xmlns='http://www.w3.org/2000/svg' class='bx--inline-notification__icon' width='20' height='20' viewBox='0 0 20 20' aria-hidden='true'><path d='M10,1c-5,0-9,4-9,9s4,9,9,9s9-4,9-9S15,1,10,1z M13.5,14.5l-8-8l1-1l8,8L13.5,14.5z'></path><path d='M13.5,14.5l-8-8l1-1l8,8L13.5,14.5z' data-icon-path='inner-path' opacity='0'></path></svg>"
                    "<div style='margin: .5rem 0;width: 100%;'>";

static const char MAINPAGE_ROW_PRINTER_BLOCK_S_STANDBY[] PROGMEM = "<div class='bx--col bx--col--auto'>"
            "<div class='bx--inline-notification bx--inline-notification--success bx--inline-notification--low-contrast'>"
                "<div class='bx--inline-notification__details'>"
                    "<svg focusable='false' preserveAspectRatio='xMidYMid meet' style='will-change: transform;' xmlns='http://www.w3.org/2000/svg' class='bx--inline-notification__icon' width='20' height='20' viewBox='0 0 20 20' aria-hidden='true'><path d='M10,1c-4.9,0-9,4.1-9,9s4.1,9,9,9s9-4,9-9S15,1,10,1z M8.7,13.5l-3.2-3.2l1-1l2.2,2.2l4.8-4.8l1,1L8.7,13.5z'></path><path fill='none' d='M8.7,13.5l-3.2-3.2l1-1l2.2,2.2l4.8-4.8l1,1L8.7,13.5z' data-icon-path='inner-path' opacity='0'></path></svg>"
                    "<div style='margin: .5rem 0;width: 100%;'>";

static const char MAINPAGE_ROW_PRINTER_BLOCK_TITLE[] PROGMEM = "<p class='bx--inline-notification__title'>%NAME% <span class='bx--tag bx--tag--gray bx--tag__label'>%API%</span></p>"
                    "<p class='bx--inline-notification__subtitle'>";


static const char MAINPAGE_ROW_PRINTER_BLOCK_LINE[] PROGMEM = "<div><strong>%T%:</strong> %V%</div>";
static const char MAINPAGE_ROW_PRINTER_BLOCK_PROG[] PROGMEM = "<div class='pStateBar'><div class='pStateBarD' style='width: %P%'>%P%</div></div>";
static const char MAINPAGE_ROW_PRINTER_BLOCK_HR[] PROGMEM = "<hr class='pStateHr'>";

static const char MAINPAGE_ROW_PRINTER_BLOCK_E[] PROGMEM = "</p></div></div></div></div>";


/**
 * @brief Class to generate HTML content from Memory
 */
class WebserverMemoryVariables {
private:
    static String rowExtraClass;

public:
    static void sendHeader(ESP8266WebServer *server, GlobalDataController *globalDataController, String pageLabel, String pageTitle);
    static void sendHeader(ESP8266WebServer *server, GlobalDataController *globalDataController, String pageLabel, String pageTitle, boolean refresh);
    static void sendFooter(ESP8266WebServer *server, GlobalDataController *globalDataController);

    static void sendMainPage(ESP8266WebServer *server, GlobalDataController *globalDataController);
    static void sendUpdateForm(ESP8266WebServer *server, GlobalDataController *globalDataController);
    static void sendWeatherConfigForm(ESP8266WebServer *server, GlobalDataController *globalDataController);
    static void sendStationConfigForm(ESP8266WebServer *server, GlobalDataController *globalDataController);
    static void sendPrinterConfigForm(ESP8266WebServer *server, GlobalDataController *globalDataController);
    static void sendSensorConfigForm(ESP8266WebServer *server, GlobalDataController *globalDataController);
    static void sendDisplayConfigForm(ESP8266WebServer *server, GlobalDataController *globalDataController);

private:
    static void sendFormCheckbox(ESP8266WebServer *server, String formId, bool isChecked, String label, bool inRow, String uniqueId);
    static void sendFormCheckbox(ESP8266WebServer *server, String formId, bool isChecked, String labelOn, String labelOff, bool inRow, String uniqueId);
    static void sendFormCheckboxEvent(ESP8266WebServer *server, String formId, bool isChecked, String label, String onChange, bool inRow, String uniqueId);
    static void sendFormCheckboxEvent(ESP8266WebServer *server, String formId, bool isChecked, String labelOn, String labelOff, String onChange, bool inRow, String uniqueId);
    static void sendFormInput(ESP8266WebServer *server, String formId, String label, String value, String placeholder, int maxLen, String events, bool isPassword, bool inRow, String uniqueId);
    static void sendFormSelect(ESP8266WebServer *server, String formId, String label, String value, String events, String options, bool inRow, String uniqueId);
    static void sendFormSubmitButton(ESP8266WebServer *server, bool inRow);
    static void sendForm(ESP8266WebServer *server, String formId, String formElement, bool inRow, String uniqueId);

    static void sendPrinterConfigFormAEModal(ESP8266WebServer *server, int id, PrinterDataStruct *forPrinter, GlobalDataController *globalDataController);

    static void sendModalDanger(ESP8266WebServer *server, String formId, String label, String title, String content, String secActionTitle, String primActionTitle, String primActionEvent);
};
