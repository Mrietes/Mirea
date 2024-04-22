
     
Base.@kwdef mutable struct Polynom{T}
    coef::Vector{T}
end
 
function ord(p::Polynom{T})::BigInt where T #вычисляет порядок многочлена, то есть наивысшую степень многочлена (равную длине вектора коэффициентов минус один).
    return length(p.coef) -1
end
 
function value(p::Polynom{T}, a::T):: T where T #вычисляет значение многочлена p в точке a.
    pval = 0
    for i in 1:length(p.coef) 
        pval+= p.coef[i]*(a^(i-1))
    end
    return pval
end
 
 
function get_max_pow(value::Int, T) # создает многочлен, где максимальная степень равна value.
    arr = []
    for i in 1:value+1
        push!(arr,T(0))
    end
    arr[end] =  T(1)
    return Polynom(arr)
end

function Base.println(p::Polynom{T}) where T #Вывод многочлена в красивой форме
    poly = ""
    if length(p.coef) != 1
        for i in 1:(length(p.coef))
            if p.coef[i] > 0
                if i == 1
                    poly *= string(p.coef[i])
                elseif i == 2
                    poly *= " + " * string(p.coef[i])*"x"
                elseif  i != length(p.coef)
                    poly*= " + " * string(p.coef[i])*"x^"*string(i-1)
                else
                    poly*= " + " * string(p.coef[i])*"x^"*string(i-1)
                end
            elseif p.coef[i] == 0
                if i == 1
                    poly *= string(0)*" + "
                elseif i == 2
                    poly *= ""
                elseif  i != length(p.coef)
                    poly*= ""
                else
                    poly*= ""
                end
            else
                if i == 1
                    poly *= string(p.coef[i])
                elseif i == 2
                    poly *= " - "*string(abs(p.coef[i]))*"x"
                elseif  i != length(p.coef)
                    poly*= " - "*string(abs(p.coef[i]))*"x^"*string(i-1)
                else
                    poly*= " - "*string(abs(p.coef[i]))*"x^"*string(i-1)
                end
            end
        end
    end
    println(poly)
end
 
 
 
function Base. +(p1::Polynom{T}, p2::Polynom{T}) where T #Дополнение +
    lst = []
    if length(p1.coef) >= length(p2.coef)
        for i in 1:length(p1.coef)
            if i <= length(p2.coef)
                push!(lst, p1.coef[i] + p2.coef[i])
            else
                push!(lst, p1.coef[i])
            end
        end
    end
    if length(p1.coef) < length(p2.coef)
        for i in 1:length(p2.coef)
            if i <= length(p1.coef)
                push!(lst, p1.coef[i] + p2.coef[i])
            else
                push!(lst, p2.coef[i])
            end
        end
    end
    return Polynom{T}(lst)
end
 
 
function Base. -(p1::Polynom{T}, p2::Polynom{T}) where T #Вычитание 
    lst= []
    if length(p1.coef) >= length(p2.coef)
        for i in 1:length(p1.coef)
            if i <= length(p2.coef)
                push!(lst, p1.coef[i] - p2.coef[i])
            else
                push!(lst, p1.coef[i])
            end
        end
    end
    if length(p1.coef) < length(p2.coef)
        for i in 1:length(p2.coef)
            if i <= length(p1.coef)
                push!(lst, p1.coef[i] - p2.coef[i])
            else
                push!(lst, -p2.coef[i])
            end
        end
    end
    return Polynom{T}(lst)
end
 
function Base. *(p1::Polynom{T}, p2::Polynom{T}) where T #умножаем два многочлена на p1 и P2
    lst = [T(0) for i in 1:(length(p1.coef)+length(p2.coef)-1)]
    for i in 1:length(p1.coef)
        for j in 1:length(p2.coef)
            lst[i+j-1] += p1.coef[i]*p2.coef[j]
        end
    end
    return Polynom{T}(lst)
end
 
function Base. *(value::T, a::Polynom) where T #умножаем многочлен на скалярное значение value
    c = Polynom(value * a.coef)
    return c
end
 
 
 
function Base. /(a::Polynom{T}, b::Polynom{T}) where T #деление полиномов a на b
    if length(a.coef)<length(b.coef)
        return 0, a
    else
        s = a
        w = b
        t::Vector{T} = []
        for i in 1:length(s.coef)
            push!(t,T(0))
        end
        while length(s.coef)>=length(b.coef)            
            coef = s.coef[end]/b.coef[end]
            poli = length(s.coef)-length(b.coef)
            q::Vector{T} = []
            for k in 1:poli
                push!(q,T(0))
            end
            push!(q,coef)
            t[length(q)] = coef
            s -= Polynom(q)*b
            if length(s.coef) !=0
                while s.coef[end]==T(0)
                    deleteat!(s.coef,length(s.coef))
                    if length(s.coef) == 0
                        break
                    end
                end  
            end
        end
    end   
    return s,Polynom(t)
end
 
println(Polynom([1, 2, 3, 5, 9, -3]))
p1 = Polynom([-6,-1,2,1]) # -6 - x - 2x^2 + x^3
p2 = Polynom([-43,4]) # -3 + x 
println((p1/p2)[2])


#struct dual{T}
    a::T
    b::T
#end
#Base. *(x::Dual{T},y::Dual{T}) where T = Dual{T}(x.a*y.a,x.b*y.a+x.a*y.b)
#Base. *(x::Dual{T},y::T) where T = Dual{T}(x.a*y, x.b*y)
#a_1+ϵ*b_1
#a_2+ϵ*b_2
#a_1+a_2+ϵ*(b_1+b_2)
#f1*f2 = a_1*a_2+ϵ*(b_1*a_2+ a_1*b_2)


