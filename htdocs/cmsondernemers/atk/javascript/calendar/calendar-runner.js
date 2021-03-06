// This function shows the calendar under the element having the given id.
// It takes care of catching "mousedown" signals on document and hiding the
// calendar if the click was outside.
function showCalendar(id, bindname, format, dateattr) 
{
  var el = document.getElementById(bindname);
  if (calendar != null) 
  {
    // we already have some calendar created
    calendar.hide();                 // so we hide it first.
  } 
  else 
  {
    // first-time call, create the calendar.
    var cal;
    if (dateattr)
    {
      // Bind calendar to dateattribute widgets
      cal = new Calendar(false, null, changeWidget, closeHandler);
    }
    else
    {
      // Bind calendar to textfield.
      cal = new Calendar(false, null, changeTextField, closeHandler);
    }
    // uncomment the following line to hide the week numbers
    // cal.weekNumbers = false;
    calendar = cal;                  // remember it in the global var
    cal.setRange(1900, 2070);        // min/max year allowed.
    cal.create();
  }
  calendar.setDateFormat(format);    // set the specified date format
  if (dateattr)
  {
    // get current value from dateattribute widgets
    calendar.parseDate(dateAttribute_getValue(id));
  }
  else
  {
    // get current value from textfield
    calendar.parseDate(el.value);      // try to parse the text in field
  }
  calendar.sel = el;                 // inform it what input field we use
  calendar.dateattrId = id;
  calendar.showAtElement(el);        // show the calendar below it

  return false;
}

// This function gets called when the end-user clicks on some date.
function changeWidget(cal, date) 
{
  //cal.sel.value = date; // just update the date in the input field.  
   dateAttribute_setValue(cal.dateattrId, date.substr(0,4), date.substr(5,2), date.substr(8,2));  
}

function changeTextField(cal, date)
{
  cal.sel.value = date; // just update the date in the input field.  
}

// And this gets called when the end-user clicks on the _selected_ date,
// or clicks on the "Close" button.  It just hides the calendar without
// destroying it.
function closeHandler(cal) 
{
  cal.hide();                        // hide the calendar
}
