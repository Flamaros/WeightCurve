function    dateDiffInDays(date1, date2)
{
    var timeDiff = Math.abs(date2.getTime() - date1.getTime());
    return Math.ceil(timeDiff / (1000 * 3600 * 24));
}

function    addDays(date, days)
{
    var result = new Date(date);
    result.setDate(date.getDate() + days);
    return result;
}
