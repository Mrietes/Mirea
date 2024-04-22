struct Polynomial{T}
    coeffs::Vector{T}
    function Polynomial{T}(coeffs) where T #конструктор структуры
        k = findfirst(x -> !iszero(x), coeffs) #[0, 0, 1] -> [1]
        return new(coeffs[k:end])
    end

    function Polynomial{T}() where T  #конструктор нулевого многочлена
        return new([])
    end
end

Base. zero(::Type{Polynomial{T}}) where T = Polynomial{T}(T[])
Base. iszero(p::Polynomial) = istmpty(p.coeff) #all(iszero, p.coeffs)
Base. isone(p::Polynomial{T}) where T = (p.coeffs == [one(T)])  #Polynomial{typeof(p.coeffs[1])}([one(p.coeffs[1])])



ord(p::Polynomial) = max(length(p.coeffs) - 1, 0) #функция, возвращающая порядок многочлена 
monomial(T, coeff, order) = begin #2x -> [2, 0] #моном многочлена 
    r = zeros(T, order + 1)
    r[1] = coeff
    return Polynomial{T}(r)
end

function Base.show(io::IO, p::Polynomial)#Вывод 
    l = length(p.coeffs)
    r = ""

    for i = 1:l
        coeff = p.coeffs[i]
        order = l - i

        if coeff < zero(coeff) && i != 1
            r = strip(r, [' ', '+'])
            r *= " - $(coeff * -1)"
        elseif coeff != Base. zero(coeff) && coeff != Base. one(coeff)
            r *= "$coeff"
        end

        if coeff != zero(coeff)
            if order == Base. one(coeff)
                r *= "x"
            elseif order != 0
                r *= "x^$order"
            end
        end
        
        r *= " + "
    end 
    
    print(io, strip(r, [' ', '+']))
end

Base. +(p::Polynomial{T}, q::Polynomial{T}) where T = begin
    n, m = length(p.coeffs), length(q.coeffs)
    if  n > m
        r = copy(p.coeffs)
        r[n-m+1:end] .+= q.coeffs
    else
        r = copy(q.coeffs)
        r[m-n+1:end] .+= p.coeffs
    end

    return Polynomial{T}(r)
end

Base. +(p::Polynomial{T}, q::T) where T = p + Polynomial{T}([q]) #перегрузка оператора +
Base. +(p::T, q::Polynomial{T}) where T = Polynomial{T}([p]) + q #перегрузка оператора +

Base. -(p::Polynomial{T}) where T = Polynomial{T}(-p.coeffs)#обратный элемент
Base. -(p::Polynomial{T}, q::Polynomial{T}) where T = p  + (-q) #перегрузка оператора -
Base. -(p::Polynomial{T}, q::T) where T = p + Polynomial{T}([-q])#перегрузка оператора -
Base. -(p::T, q::Polynomial{T}) where T = Polynomial{T}([p]) - q#перегрузка оператора -

Base. *(p::Polynomial{T}, q::Polynomial{T}) where T = begin #Перегрузка оператора *
    n, m = ord(p), ord(q)
    l = n + m
    r = zeros(T, l + 1)
    for i = 1:n+1
        for j =1:m+1
            k = i + j - 1
            r[k] += p.coeffs[i] * q.coeffs[j]
        end
    end

    return Polynomial{T}(r)
end
Base. *(p::Polynomial{T}, q::T) where T = Polynomial{T}(q * p.coeffs) #Перегрузка оператора *
Base. *(p::T, q::Polynomial{T}) where T = Polynomial{T}(p * q.coeffs) #Перегрузка оператора *

Base. divrem(p::Polynomial{T}, q::Polynomial{T}) where T = begin# деление (целая часть и остаток)
    r = Polynomial{T}()
    n, m = ord(p), ord(q)
    while n >= m
        coeff = p.coeffs[1] / q.coeffs[1]
        order = n - m
        a = monomial(T, coeff, order)
        r += a
        p = p - (q * a)
        n = ord(p)
    end

    return r, p
end

Base. /(p::Polynomial{T}, q::Polynomial{T}) where T = divrem(p, q)[1]# деление (целая часть)
Base. %(p::Polynomial{T}, q::Polynomial{T}) where T = divrem(p, q)[2]#Деление (остаток)

#x^3 + x^2 + x + 1
#((x + 1)x + 1)x + 1
#((ax + b)x + c)x + d
function (p::Polynomial)(x)
    n = ord(p)
    res = p.coeffs[1]
    for i = 2:n+1
        res = res * x + p.coeffs[i]
    end
    return res
end

derivative(p::Polynomial{T}) where T = begin #Вычисление проиводной многочлена 
    #ax^n + bx^(n - 1) + ... + cx^2 + dx + e
    #an * x^(n - 1) + b(n - 1) * x^(n - 2) + ... + 2cx + d
    n = ord(p)
    r = zeros(T, n)
    for i = 1:n
        a = p.coeffs[i]
        r[i] = a * n
        n -= 1
    end
    return Polynomial{T}(r)
end

valdiff(p::Polynomial{T}, x) where T = begin #вернуть значение многочлена и его производной в заданной точке
    n = ord(p)
    r = zero(T)
    dr = zero(T)
    for i = 1:n+1
        dr = dr * x + r
        r = r * x + p.coeffs[i]
    end

    return (r, dr)
end


v1 = [7//1, 0//1, -5//1, 1//1, 2//1]
p1 = Polynomial{Rational}(v1)

v2 = [7//1, 0//1, -1//1]
p2 = Polynomial{Rational}(v2)

println("($p1) * ($p2) = $(p1 * p2)")

dr = divrem(p1, p2)
println(dr)

p3 = Polynomial{Int32}([1, 1, -1, -1])
println(p3(-2))

p4 = Polynomial{Int32}([2, -2, 3])