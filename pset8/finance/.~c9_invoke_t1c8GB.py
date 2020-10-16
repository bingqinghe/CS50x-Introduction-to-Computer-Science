import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # get the info from the database
    stock = db.execute("SELECT * FROM stocks WHERE user_id = :user", user=session['user_id'])
    cash = db.execute("SELECT cash FROM users WHERE id = :user", user=session['user_id'])[0]['cash']

    # pass the info to the templates and shown on the homepage
    total = cash
    stocks = []
    for index, row in enumerate(stock):
        stock_info = lookup(row['symbol'])
        stocks.append(list((stock_info['symbol'], stock_info['name'], stock_info['price'], row['amount'], round(stock_info['price'] * row['amount'], 2))))
        total += stocks[index][4]
    # template to the index.html
    return render_template("index.html", stocks=stocks, cash=round(cash, 2), total=round(total, 2))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # should be POST first otherwise refresh
    if request.method == 'POST':
        # transaction requires: stock, amount
        symbol = lookup(request.form.get('symbol'))['symbol']
        amount = int(request.form.get('amount'))
        # not valid symbol
        if not lookup(symbol):
            return apology("Stock not found")

        # calculate the transaction
        price = lookup(symbol)['price']
        cash = db.execute("SELECT cash FROM users WHERE id = :user", user=session['user_id'])[0]['cash']
        # the money left in the user's account
        cash_left = cash - price * float(amount)
        # make sure cash left in the account is positive
        if cash_left < 0:
            return apology("Sorry you don't have enough money for this transaction")

        # new / existing stock in the account
        stock = db.execute("SELECT amount FROM stocks WHERE user_id = :user AND symbol = :symbol", user=session['user_id'], symbol=symbol)
        # new, insert new row in db
        if not stock:
            db.execute("INSERT INTO stocks(user_id, symbol, amount) VALUES(:user, :symbol, :amount)", user=session['user_id'], symbol=symbol, amount=amount)
        # exist, update the existing colomn in the specific row
        else:
            amount += stock[0]['amount']
            db.execute("UPDATE stocks SET amount = :amount WHERE user_id = :user AND symbol = :symbol", user=session['user_id'], symbol=symbol, amount=amount)

        # update cash in the users db
        db.execute("UPDATE users SET cash = :cash_after WHERE id = :user", user=session['user_id'], cash_after=cash_after)
        # update transaction history
        db.execute("INSERT INTO transactions(user_id, symbol, amount, value) VALUES(:user, :symbol, :amount, :value)", user=session['user_id'], symbol=symbol, amount=amount, value=-round(price * float(amount)))

        flash("Buy Successfully")
        return redirect('/')
    # not valid user request
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # get the trans rows first
    trans = db.execute("SELECT * FROM transactions WHERE user_id = :user", user=session['user_id'])
    trans_list = []
    for row in trans:
        stock_info = lookup(row['symbol'])
        trans_list.append(list((stock_info['symbol'], stock_info['name'], row['amount'], row['value'], row['date'])))

    return render_template("history.html", transactions=trans_list)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == 'POST':
        # valid request from user
        stock = lookup(request.form.get('symbol'))
        # invalid stock lookup
        if not stock:
            return apology("Stock not found")
        return render_template('quote.html', stock=stock)
    else:
        return render_template("quote.html", stock="")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # clean the previous session
    session.clear()
    # should use POST to input and store
    if request.method == 'POST':
        # username not provided
        if not request.form.get('username'):
            return apology("Please provide username", 403)
        # password not provided
        elif not request.form.get('password'):
            return apology("Please provide password", 403)
        # password not match
        elif request.form.get('password') != request.form.get('confirm-password'):
            return apology("Password not matched", 403)
        # usename being used
        elif db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get('username')):
            return apology("Username already being used", 403)

        # all information correct
        # insert into db
        db.execute("INSERT INTO users(username, hash) VALUES(:username, :hash)", username=request.form.get('username'), hash=generate_password_hash(request.form.get('password')))
        # get the info from db
        user_info = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get('username'))
        # store into session
        session['user_id'] = user_info[0]['id']
        return redirect('/')
    else:
        return render_template('register.html')


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # request method == POST
    if request.method == 'POST':
        # info requires: symbol, amount, price
        symbol = request.form.get('symbol')
        price = lookup(symbol)['price']
        amount = int(request.form.get('amount'))
        value = round(price * float(amount))
        # update stocks
        amt_before = db.execute('SELECT amount FROM stocks WHERE user_id = :user AND symbol = :symbol', user=session['user_id'], symbol=symbol)[0]['amount']
        amt_after = amt_before - amount
        # amount error check
        # no more this stock, delete
        if amt_after == 0:
            db.execute('DELETE FROM stocks WHERE user_id = :user AND symbol = :symbol', user=session['user_id'], symbol=symbol)
        # not enough amount holder
        elif amt_after < 0:
            return apology("Sorry you don't own enough stocks")
        # valid stock holder
        # update to the stock db new own number
        else:
            db.execute('UPDATE stocks SET amount = :amount WHERE user_id = :user AND symbol = :symbol', user=session['user_id'], symbol=symbol, amount=amt_after)

        # update cash db
        cash = db.execute('SELECT cash FROM users WHERE id = :user', user=session['user_id'])[0]['cash']
        cash_after = cash + value
        db.execute('UPDATE users SET cash = :cash WHERE id = ')

        # update history db
        db.execute('INSERT INTO transactions(user_id, symbol, amount, value) VALUES(:user, :symbol, :amount, :value)', user=session['user_id'], symbol=symbol, amount=-amount, value=value)

        flash('Sell Successfully')
        return redirect('/')
    # request method == GET
    else:
        stock_info = db.execute('SELECT symbol, amount FROM stocks WHERE user_id = :user', user=sesion['user_id'])
        stocks = {}

        for row in stock_info:
            stocks[row['symbol']] = row['amount']

        return render_template('sell.html', stocks=stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
